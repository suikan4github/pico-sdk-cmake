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

# subdir_sdk構成
この構成では、algorithm ディレクトリに加え、pico-sdk を参照する
driver ディレクトリを新設し、gpio_init.* をそこに移しています。

考え方としては algorithm ディレクトリを作ったときと同じですが、
driver の中から pico-sdk にアクセスする点が違います。

pico-sdk の CMakeLists.txt は
複雑な前処理や後処理を自動的でやってくれて便利なのですが、あまりに
うまく複雑さを遮蔽しているために困ることがあります。
この場合がそうで、pico-sdk を使う開発では pico-sdk にアクセスする
ライブラリのビルドは一筋縄ではいきません。

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
    ├── driver
    │   ├── CMakeLists.txt
    │   ├── initgpio.c
    │   └── initgpio.h
    └── main.c
```
CMakeのルールに従い、driver ディレクトリにも CMakeLists.txt を
置いています。

ルート・ディレクトリの CMakeLists.txt は、[src](https://github.com/suikan4github/pico-sdk-cmake/tree/src) 構成と同じですので説明を省略します。

新しく作った src/driver/CMakeLists.txt は見慣れない形をしています。

```CMake
# スタティック・リンク・ライブラリ algorithm を作る。
# 第２変数がINTERFACEのとき、ファイル名を引数として取らないことに注意
add_library(driver INTERFACE)

# driver ライブラリが initgpio.c からなることを宣言する。
target_sources( driver INTERFACE 
            initgpio.c)

# driver ライブラリが pico_stdlibに依存していることを宣言する。
target_link_libraries(driver INTERFACE
                        pico_stdlib)
```

add_library() が [subdir](https://github.com/suikan4github/pico-sdk-cmake/tree/subdir) 構成とずいぶん違います。

subdir 構成では STATIC 宣言をしてスタティック・リンク・ライブラリを作っていました。
しかし、この構成では INTERFACE 宣言をしています。そしてファイル名を引数にとりません。

add_library() がファイル名を引数にとらないので、target_souces() 命令を使って
driver ライブラリを構成するのは initgpio.c ファイルだと宣言しています。

そして target_link_library() は INTERFACE 引数を使って driver ライブラリが
pico_stdlib ライブラリを使うと宣言しています。依存関係を明示的に示すことでビルドを並列に行う可能性が増えます。
逆に言うと、この宣言をしない場合はビルドがやや遅くなる以外の
問題は起きません。

src/CMakeLists.txt は、この構成でも記述が増えました。
```CMake
# blinkプログラムがmain.cとinit.cからなることを宣言する
# add_executable()で宣言した名前(blink)は実行ファイルの名前
# になる。
add_executable(blink
        main.c
        )

# blinkプログラムがpico_stdlibライブラリを使うことを宣言する。
target_link_libraries(blink PUBLIC
                        algorithm
                        driver
                        pico_stdlib)

# サブディレクトリalgorithmをインクルード・パスに追加する
target_include_directories(blink PUBLIC 
                        algorithm 
                        driver)

# pico-sdkが用意した関数
# blinkプログラムからブートローダ形式のファイルを作る
# ブートローダーファイル(blink.uf2)はbuildディレクトリに
# 出力される
pico_add_extra_outputs(blink)

# サブディレクトリをビルドに追加する
add_subdirectory(algorithm)
add_subdirectory(driver)


```

gpioinit.c が driver サブディレクトリに移ったため、
add_executableのリストから消えています。今や add_executable() のソース・コードは main.c だけです。

target_link_librraries と target_include_directories に
driver が追加されました。また、 add_subdirectory() で
driver をビルドするモジュールとして追加しています。

ブートローダーファイルは build/src/blink.uf2 です。これは subdir 構成と変わりません。

さて、先に変更点だけ示しましたが、INTERFACE ライブラリとは
何でしょうか。

CMake の INTERFACE ライブラリは、実体を持たないライブラリです。add_library() で　STATIC と宣言された
ライブラリはスタティック・リンキング・ライブラリとして実体が生成されます。
そして、ビルドは当該ディレクトリで行われます。

しかしながら、INTERFACE ライブラリは実体がないため、当該
ディレクトリではコンパイルが行われません。単にソース・ファイルが
記録されるだけです。そして、INTERFACE ライブラリに依存する
実行プログラムのビルド時に、実行プログラムのソースと一緒に
コンパイルされます。

普通はこんな奇妙なことをする必要はありません。ところが、pico-sdk の CMake では必要になります。pico-sdk の CMake は
SDK のインクルードディレクトリを add_executable() にだけ
渡します。そして、add_library() を行うときにそのパスを知る
方法はありません。そのため、add_library( STATIC ) をつかって SDK 
依存のコードをコンパイルすると、インクルードファイルが見つからずにエラー終了してしまうので。

CMake のINTERFACE ライブラリという奇妙な仕掛けを使うのは
このためです。INTERFACE ライブラリのソース・ファイルは
コンパイルが add_executable() のソース・ファイルと同じ
文脈で行われます。そのため、SDK のインクルード・パスを
受け取ることができ、コンパイルが無事終了します。

pico-sdk を使って大きめのプログラムを作ろうとすると、
このことを知っておく必要があります。

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