include_guard()

if(VOXER_VOICES_LIST_PROCESSED)
    return()
endif()

set(VOXER_VOICES_LIST_DIR
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/voices"
    CACHE STRING "Base dir of voices")
set(VOXER_VOICES_MODEL_BASE_URL
    "https://huggingface.co/rhasspy/piper-voices/resolve/v1.0.0/<lang-code>/<lang>/<name>/<quality>/<lang>-<name>-<quality>.onnx?download=true"
    CACHE STRING "The base URL to download model")
set(VOXER_VOICES_CONFIG_BASE_URL
    "https://huggingface.co/rhasspy/piper-voices/resolve/v1.0.0/<lang-code>/<lang>/<name>/<quality>/<lang>-<name>-<quality>.onnx.json?download=true.json"
    CACHE STRING "The base URL to download config")

function(CreateUrl baseUrl lang name quality url)
    string(SUBSTRING "${lang}" 0 2 langCode)
    string(REPLACE "<lang-code>"
            "${langCode}" baseUrl "${baseUrl}")
    string(REPLACE "<lang>"
            "${lang}" baseUrl "${baseUrl}")
    string(REPLACE "<name>"
            "${name}" baseUrl "${baseUrl}")
    string(REPLACE "<quality>"
            "${quality}" baseUrl "${baseUrl}")
    set(${url} ${baseUrl} PARENT_SCOPE)
endfunction()

function(DownloadVoice lang name quality)
    CreateUrl(${VOXER_VOICES_MODEL_BASE_URL}
        ${lang} ${name} ${quality} modelUrl)
    CreateUrl(${VOXER_VOICES_CONFIG_BASE_URL}
            ${lang} ${name} ${quality} configUrl)

    file(MAKE_DIRECTORY "${VOXER_VOICES_LIST_DIR}")
    set(modelOutputFile
        "${VOXER_VOICES_LIST_DIR}/${lang}-${name}-${quality}.onnx")
    set(configOutputFile
        "${VOXER_VOICES_LIST_DIR}/${lang}-${name}-${quality}.onnx.json")

    file(DOWNLOAD
        "${modelUrl}" "${modelOutputFile}" SHOW_PROGRESS)
    file(DOWNLOAD
        "${configOutputFile}" "${configOutputFile}" SHOW_PROGRESS)
endfunction()

function(DownloadVoices downloadVoices)
    file(READ "${CMAKE_CURRENT_LIST_DIR}/voices.csv" voices)
    string(REPLACE "\n" ";" voices "${voices}")
    foreach(voice IN LISTS voices)
        string(REPLACE "," ";" attrs "${voice}")
        list(POP_FRONT attrs attr1)
        list(POP_FRONT attrs attr2)
        list(POP_FRONT attrs attr3)
        if(attr1)
            list(FIND downloadVoices "${attr1}+${attr2}" index)
            if(NOT index EQUAL -1)
                DownloadVoice(${attr1} ${attr2} ${attr3})
            endif()
        endif()
    endforeach()
endfunction()

DownloadVoices("${VOXER_VOICES_LIST}")

set(VOXER_VOICES_LIST_PROCESSED ON
    CACHE BOOL "Indicator whether voices are downloaded")
