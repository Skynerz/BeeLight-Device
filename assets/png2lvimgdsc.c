#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define HEADER (const char *)"#ifdef __has_include\n\
#if __has_include(\"lvgl.h\")\n\
#ifndef LV_LVGL_H_INCLUDE_SIMPLE\n\
#define LV_LVGL_H_INCLUDE_SIMPLE\n\
#endif\n\
#endif\n\
#endif\n\
\n\
#if defined(LV_LVGL_H_INCLUDE_SIMPLE)\n\
#include \"lvgl.h\"\n\
#else\n\
#include \"lvgl/lvgl.h\"\n\
#endif\n\
\n\
#ifndef LV_ATTRIBUTE_MEM_ALIGN\n\
#define LV_ATTRIBUTE_MEM_ALIGN\n\
#endif\n\
\n\
#ifndef LV_ATTRIBUTE_%s\n\
#define LV_ATTRIBUTE_%s\n\
#endif\n\
\n\
static const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_%s\n"

#define VAR_NAME (const char *) "const uint8_t %s_map[] = {\n"
//__attribute__ ((section (\"ICONS\"))) TODO linker file ?
#define MAP_STRUCT (const char*) "const lv_image_dsc_t %s = {\n\
    .header = {\n\
        .cf = LV_COLOR_FORMAT_RAW_ALPHA,\n\
        .w = %d,\n\
        .h = %d,\n\
    },\n\
    .data_size = sizeof(%s_map),\n\
    .data = %s_map,\n\
};\n"

#define FILENAMELEN 64
#define BUFFERSIZE  1024

int main(int argc, char **argv)
{
    FILE *in = NULL, *out = NULL;
    char *inputFileName;
    char *outputFilename;
    char outputFile[FILENAMELEN];
    char varName[FILENAMELEN]; //based on filename
    char defineName[FILENAMELEN] = {0}; //based on filename
    char outBuffer[BUFFERSIZE];
    uint8_t inBuffer[BUFFERSIZE];
    size_t readSz;

    //png infos
    uint8_t pngHeaderRead = 0;
    uint32_t w, h;
    
    if (argc == 2)
    {
        inputFileName = argv[1];
        in = fopen(inputFileName, "rb");
        if (in != NULL)
        {
            outputFilename = basename(inputFileName);
            strncpy(outputFile, outputFilename, sizeof(outputFile));
            char *tok = strchr(outputFile, '.');
            if (tok != NULL)
                *tok = 0;

            strncpy(varName, outputFile, sizeof(varName));
            snprintf(outputFile, sizeof(outputFile), "%s.c", varName);
            printf("Output file: %s\r\n", outputFile);
            out = fopen(outputFile, "wb+");

            for (uint8_t i = 0; i < strlen(varName); i++)
            {
                defineName[i] = toupper(varName[i]);
            }
            
            snprintf(outBuffer, sizeof(outBuffer), HEADER, defineName, defineName, defineName);
            fwrite(outBuffer, strlen(outBuffer), 1, out);

            snprintf(outBuffer, sizeof(outBuffer), VAR_NAME, varName);
            fwrite(outBuffer, strlen(outBuffer), 1, out);

            //IHDR toujours present ?
            while((readSz = fread(inBuffer, 1, sizeof(inBuffer), in)) > 0) {
                if(!pngHeaderRead) {
                    for(uint8_t i = 0 ; i < 4 ; i++){
                        ((uint8_t*)&w) [i] = inBuffer[19 - i];
                    }
                    for(uint8_t i = 0 ; i < 4 ; i++){
                        ((uint8_t*)&h) [i] = inBuffer[23 - i];
                    }
                    printf("img size %xx%x\r\n", w, h);
                    
                    pngHeaderRead = 1;
                }
                for (size_t i = 0; i < readSz; i++)
                {
                    snprintf(outBuffer, sizeof(outBuffer), "0x%02x, ", inBuffer[i]);
                    if (i % 16 == 0 && i)
                    {
                        strcat(outBuffer, "\r\n    ");
                    }
                    fwrite(outBuffer, strlen(outBuffer), 1, out);
                }
            }
            snprintf(outBuffer, sizeof(outBuffer), "};\r\n\r\n");
            fwrite(outBuffer, strlen(outBuffer), 1, out);

            snprintf(outBuffer, sizeof(outBuffer), MAP_STRUCT, varName, w, h, varName, varName);
            fwrite(outBuffer, strlen(outBuffer), 1, out);
            fclose(out);
            fclose(in);
        }
    }
}