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

# src構成
この構成は、すべてのソース・ファイルをsrcディレクトリに格納しています。

ソース・ファイルをルート・ディレクトリの下のディレクトリに
格納する構成はオーソドックスと言えます。

```
.
├── CMakeLists.txt
├── LICENSE
├── pico_sdk_import.cmake
├── README.md
└── src
    ├── CMakeLists.txt
    ├── getduration.c
    ├── getduration.h
    ├── initgpio.c
    ├── initgpio.h
    └── main.c
```
CMakeのルールに従い、ルート・ディレクトリとsrcディレクトリの双方に
CMakeLists.txt が必要です。

ルート・ディレクトリの CMakeLists.txt の後半は以下のようになっています。
```CMake
# --------------------------------------------------
# 以下が例題ごとに変わる

# srcサブディレクトリをプロジェクトに加える
add_subdirectory("src")
```
一行だけになりました。

add_subdirectory() は CMake の命令です。この命令は指定したサブディレクトリを
プロジェクトに追加します。プロジェクトに追加するのであって、インクルード
しているわけではないことに注意してください。

CMakeでは原則として各ディレクトリの CMakeLists.txt がそのディレクトリでの
ビルドのルールを定義します。add_subdirectory() は、指定したディレクトリでの
ビルドを、プロジェクトのビルドに組み込む命令です。

src/CMakeLists.txt は以下のようになっています。
```CMake
# blinkプログラムがmain.cとinit.cからなることを宣言する
# add_executable()で宣言した名前(blink)は実行ファイルの名前
# になる。
add_executable(blink
        main.c
        initgpio.c
        getduration.c
        )

# blinkプログラムがpico_stdlibライブラリを使うことを宣言する。
target_link_libraries(blink pico_stdlib)

# pico-sdkが用意した関数
# blinkプログラムからブートローダ形式のファイルを作る
# ブートローダーファイル(blink.uf2)はbuildディレクトリに
# 出力される
pico_add_extra_outputs(blink)

```

これは、[basic](https://github.com/suikan4github/pico-sdk-cmake/tree/basic) 構成のCMakeLists.txtの後半部分そのものです。

CMakeLists.txt の内容は変わりませんが、pico_add_extra_outputs() が
src ディレクトリに移ったため、生成物のディレクトリも変わります。

具体的には、ブートローダーファイルは build/src/blink.uf2 になります。

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