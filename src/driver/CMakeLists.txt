# スタティック・リンク・ライブラリ algorithm を作る。
# 第２変数がINTERFACEのとき、ファイル名を引数として取らないことに注意
add_library(driver INTERFACE)

# driver ライブラリが initgpio.c からなることを宣言する。
target_sources( driver INTERFACE initgpio.c)

# このディレクトリをalgorithmのインクルード・パスに追加する
# algorithmを使うターゲットのビルドに使用される。
target_include_directories(driver INTERFACE . )

# driver ライブラリが pico_stdlibに依存していることを宣言する。
target_link_libraries(driver INTERFACE pico_stdlib)

