# blinkプログラムがmain.cとinit.cからなることを宣言する
# add_executable()で宣言した名前(blink)は実行ファイルの名前
# になる。
add_executable(blink
        main.c
        initgpio.c
        )

# blinkプログラムがpico_stdlibライブラリを使うことを宣言する。
target_link_libraries(blink
                        algorithm
                        pico_stdlib)

# サブディレクトリalgorithmをインクルード・パスに追加する
target_include_directories(blink PUBLIC 
                        algorithm )

# pico-sdkが用意した関数
# blinkプログラムからブートローダ形式のファイルを作る
# ブートローダーファイル(blink.uf2)はbuildディレクトリに
# 出力される
pico_add_extra_outputs(blink)

# algorithm サブディレクトリをビルドに追加する
add_subdirectory(algorithm)


