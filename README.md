# pico-sdk-cmake
Explain how CMake should be configured for the RasPi Pico development with pico-sdk

# 詳細

このプロジェクトはpico-sdkを使ったRaspberry Pi Pico開発について、ディレクトリ構成の面から説明留守ためのものです。

pico-sdk固有の癖とcmakeの理解しにくさが組み合わさっているため、
Raspberry Pi Picoのプログラム開発はディレクトリ構成で躓きがち
です。

このプロジェクトではgitの以下のタグを使って構成を調べることができます。

タグ       | 説明
-----------|-----------
basic      | すべてのソース・ファイルをプロジェクトルートに置きます
src        | すべてのソース・ファイルをsrcディレクトリに置きます
subdir     | 一部のソース・ファイルをサブディレクトリに置きます
subdir_sdk | サブディレクトリに置いたファイルからSDKを参照します

各々のタグのREADMEではその構成の解説を読むことができます。

# basic構成
この構成では、すべてのソース・ファイルとヘッダー・ファイルがプロジェクトの
ルートディレクトリに置かれています。構成としてはシンプルですが、ルートディレクトリは
ごちゃごちゃしてしまいます。

特にOSSの場合、プロジェクトのルートディレクトリはREADMEやCHANGELOGといった
プロジェクト自身の説明をするファイルが置かれています。ここにソースを全部置くのは
あまり良い方法ではありません。

CMakeFileは前半と後半に分かれています。前半はすべての構成に共通であり、
SDKのダウンロードとビルドの前の各種設定を行っています。ユーザーがこの部分に
手を入れるのはプロジェクト名を変更するときだけです。

```CMake
# プロジェクト名の宣言
project("my_project")
```

CMakeLists.txtの後半は以下のようになっています。
```CMake
# --------------------------------------------------
# 以下が例題ごとに変わる

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
ここには特にひねりはありません。


add_executable()はCMakeの関数で、blinkという
ターゲットプログラムのソースコードがmainc, initgpio.c, getduration.cの３つ
であることを宣言しています。

target_link_librariyはblinkプログラムがpico-sdkのpico_stdlibライブラリを使用している
（pico_stdlibライブラリに依存している）ことを宣言しています。

pico_add_extra_outputs()はpico-sdkの関数です。この関数はblinkプログラムに対して
実行ファイル(ELF)だけではなく、ブートストラップ・ロード用のファイルも作るよう宣言
しています。


# ビルド
このプロジェクトは以下の環境でビルド可能です。

- Ubuntu 22.04
- Windows 11
- Ubuntu 22.04 on WSL

CMakeがビルド中に[pico-sdk](https://github.com/raspberrypi/pico-sdk)をダウンロードするため、あらかじめpico-sdkをインストールしておく必要はありません。

環境の設定方法は[pico-env-easy](https://github.com/suikan4github/pico-env-easy)を参照してください。

ビルドはコマンドラインから
```bash
mkdir build
cd build
cmake ..
cd ..
cmake --build build
```
で行えます。また、Visual Studio Codeからもビルドできます。

# ライセンス

このプロジェクトは[MIT ライセンス](LICENSE)にて公開しています。

なお、[pico_sdk_import.cmake](pico_sdk_import.cmake)および[blink.c](blink.c)はRaspberryPiの[pico-sdk](https://github.com/raspberrypi/pico-sdk) v1.5.1よりコピーしたものです。このファイルはBSD 3-Clause "New" or "Revised" Licenseが定める要件により、Raspberry Pi (Trading) Ltd.よりライセンスされています。詳しくは当該ファイルのライセンス要件を読んでください。