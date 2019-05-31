set(FontFiles
    ${CMAKE_CURRENT_SOURCE_DIR}/arial.ttf
    ${CMAKE_CURRENT_SOURCE_DIR}/consola.ttf
    ${CMAKE_CURRENT_SOURCE_DIR}/lucon.ttf
    ${CMAKE_CURRENT_SOURCE_DIR}/wqy-microhei.ttc
    ${CMAKE_CURRENT_SOURCE_DIR}/roboto.ttf
    )

set(Fonts_Directory
    ${CMAKE_BINARY_DIR}/bin/fonts/
    )

foreach( Create_Dir ${Fonts_Directory} )
    file(MAKE_DIRECTORY ${Create_Dir})
endforeach()

add_custom_target(fonts2bin  ALL
    COMMAND  echo ${FontFiles}
    COMMENT  " ${FontFiles} \n are prepped..."
    SOURCES  ${FontFiles}
    )

foreach( font  ${FontFiles} )
    add_custom_command( TARGET  fonts2bin POST_BUILD
        COMMAND  ${CMAKE_COMMAND} -E  copy  ${font}  ${CMAKE_BINARY_DIR}/bin/fonts/
        #COMMAND  ${CMAKE_COMMAND} -E  copy  ${font}  ${CMAKE_BINARY_DIR}/bin/Debug/fonts/
        #COMMAND  ${CMAKE_COMMAND} -E  copy  ${font}  ${CMAKE_BINARY_DIR}/bin/Release/fonts/
        COMMENT  "copying ${font} ...")
endforeach()


#CopyDataFiles(fonts2bin  FontFiles  $<TARGET_FILE_DIR:${exe_name}>)


#######################################
