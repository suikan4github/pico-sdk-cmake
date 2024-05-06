# スタティック・リンク・ライブラリ algorithm を作る。
# ソースファイルは getduration.c .
add_library(algorithm STATIC
    getduration.c)


# このディレクトリをalgorithmのインクルード・パスに追加する
# algorithm自身のビルドと、algorithmを使うターゲットのビルドの
# 双方に使用される。
target_include_directories(algorithm PUBLIC . )
