# pico-sdk-cmake
Explain how CMake should be configured for the RasPi Pico development with pico-sdk

# 詳細

このプロジェクトは[pico-sdk](https://github.com/raspberrypi/pico-sdk)を使ったRaspberry Pi Pico開発について、ディレクトリ構成の面から説明するためのものです。

pico-sdk固有の癖とcmakeの理解しにくさが組み合わさっているため、
Raspberry Pi Picoのプログラム開発はディレクトリ構成で躓きがちです。

そこで、このプロジェクトではgitブランチを使って複数の構成を
紹介し、それぞれについて説明します。

ブランチ       | 説明
-----------|-----------
[basic](https://github.com/suikan4github/pico-sdk-cmake/tree/basic)      | すべてのソース・ファイルをプロジェクトルートに置きます
[src](https://github.com/suikan4github/pico-sdk-cmake/tree/src)        | すべてのソース・ファイルをsrcディレクトリに置きます
[subdir](https://github.com/suikan4github/pico-sdk-cmake/tree/subdir)     | 一部のソース・ファイルをサブディレクトリに置きます
[subdir_sdk](https://github.com/suikan4github/pico-sdk-cmake/tree/subdir_sdk) | サブディレクトリに置いたファイルからSDKを参照します

各々のタグのREADMEではその構成の解説を読むことができます。

# すべての構成に共通の構造


プロジェクト・ルートの CMakeLists.txt は前半と後半に分かれています。前半はすべての構成に共通であり、
SDKのダウンロードとビルドの前の各種設定を行っています。
ユーザーがこの部分に手を入れるのはプロジェクト名を変更するときだけです。

```CMake
# プロジェクト名の宣言
project("my_project")
```

pico_sdk_import.cmakeはpico-sdkからコピーしたものであり、これによって
CMakeコンフィギュレーション時にpico-sdkを自動的に取得できます。

詳しくはpico-sdkを参照してください。

# subdir構成
この構成では、アルゴリズム関連ソース・ファイルを格納する algorithm 
ディレクトリを新設し、getduration.* をそこに移しています。

プロジェクトが大きくなると、このようにモジュールごとのサブディレクトリを
作ることになります。なお、getduration.c はアルゴリズムのみ含み、
pico-sdk の関数にはアクセスしません。

```
.
├── CMakeLists.txt
├── LICENSE
├── pico_sdk_import.cmake
├── README.md
└── src
    ├── algorithm
    │   ├── CMakeLists.txt
    │   ├── getduration.c
    │   └── getduration.h
    ├── CMakeLists.txt
    ├── initgpio.c
    ├── initgpio.h
    └── main.c
```
CMakeのルールに従い、algorithm ディレクトリにも CMakeLists.txt を
置いています。

ルート・ディレクトリの CMakeLists.txt は、[src](https://github.com/suikan4github/pico-sdk-cmake/tree/src) 構成と同じですので
説明を省略します。

新しく作った src/algorithm/CMakeLists.txt はとても簡単です。

```CMake
# スタティック・リンク・ライブラリ algorithm を作る。
# ソースファイルは getduration.c .
add_library(algorithm STATIC
    getduration.c)
```

add_library() は CMake の命令です。この命令は指定したファイルを集めて
ライブラリにします。最初の引数でライブラリの名前が algorithm であると
宣言しています。2番目の引数でスタティック・リンク・ライブラリであると
宣言しています。


src/CMakeLists.txt は記述が増えました。
```CMake
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
```

getduration.c が algorithm サブディレクトリに移ったため、
add_executableのリストから消えています。

一方、3つほど追加があります。

1. target_link_libraries() に algorithm ライブラリが増えています。これは blink プログラムのビルドに algorithm ライブラリが必要であるためです。
1. target_include_directories() 命令が新しく加わりました。この命令では
blink プログラムのビルド時に、algorithm ディレクトリを含めるよう指示しています。
1. add_subdirectory() で、algorithm ディレクトリをビルドに追加しています。

ややごちゃごちゃしているように見えますが、CMake による階層ディレクトリの
管理方法としては普通です。algorithm ディレクトリにソース・ファイルを
追加する場合は、 src/algorithm/CMakeLists.txt の add_library に
ファイル名を追加します。

ブートローダーファイルは build/src/blink.uf2 です。

# ビルド
このプロジェクトは以下の環境でビルド可能です。
開発環境の設定方法は[pico-env-easy](https://github.com/suikan4github/pico-env-easy)を参照してください。

- Ubuntu 22.04
- Windows 11
- Ubuntu 22.04 on WSL

CMakeがビルド中に[pico-sdk](https://github.com/raspberrypi/pico-sdk)をダウンロードするため、あらかじめpico-sdkをインストールしておく必要はありません。


ビルドはコマンドラインから以下のコマンドを実行します。
```bash
mkdir build
cd build
cmake ..
cd ..
cmake --build build
```
また、Visual Studio Codeからもビルドできます。

# ライセンス

このプロジェクトは[MIT ライセンス](LICENSE)にて公開しています。

なお、[pico_sdk_import.cmake](pico_sdk_import.cmake)はRaspberryPiの[pico-sdk](https://github.com/raspberrypi/pico-sdk) v1.5.1よりコピーしたものです。このファイルはBSD 3-Clause "New" or "Revised" Licenseが定める要件により、Raspberry Pi (Trading) Ltd.よりライセンスされています。詳しくは当該ファイルのライセンス要件を読んでください。