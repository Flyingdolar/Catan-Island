# Development TIPS

[![link](https://img.shields.io/badge/回到_Readme-eee.svg?style=for-the-badge)](README.md)

> Reference to `Howard Guo - 開發過程體醒事項`

## Git SSH 設定

1. **先到電腦的根目錄產生 SSH Key**

    ```git
    ssh-keygen -t rsa -C <信箱>
    ```

    會產生兩個檔案，一個是 `id_rsa`，另一個是 `id_rsa.pub`，其中 `id_rsa` 是私鑰，`id_rsa.pub` 是公鑰

2. **將公鑰加入 GitHub 帳號**

    1. 先到 GitHub 帳號的設定頁面
    2. 點選左邊的 SSH and GPG keys
    3. 點選右上角的 New SSH key
    4. 將 `id_rsa.pub` 的內容複製貼上
    5. 點選 Add SSH key

    你的電腦之後就可以透過 SSH 連線到 GitHub 了，
    而且不管換到哪個專案都一樣，因為是電腦綁定 Github 帳戶

## 初次使用

1. **設定使用者名稱與信箱**
   - `git config --global user.name <使用者名稱>`
   - `git config --global user.email <信箱>`

2. **設定編輯器**

    ```git
    git config --global core.editor <編輯器名稱>
    ```

    | 編輯器名稱 | 說明 |
    | --- | --- |
    | `vim` | 預設編輯器 vim |
    | `nano` | 簡易編輯器 nano |
    | `joe` | 紀老師上課最愛用的編輯器 |
    | `code` | VSCode 編輯器 |

   備註：如果是使用 VSCode，記得設定時要加上 `--wait`，否則 commit 時會直接結束指令、操作失敗

3. **手動設定 git 的方式**

    ```git
    git config --global -e
    ```

    會跳出編輯器，請依照下方格式填寫

    ```git
    [user]
        name = <使用者名稱>
        email = <信箱>
    [core]
        editor = <編輯器名稱>
    ```

4. **一些原理與解釋**
   1. `--global` 代表的是全域設定，會將設定寫入 `~/.gitconfig` 檔案中
   2. 沒有 `--global` 代表的是專案設定，會將設定寫入專案目錄下的 `.git/config` 檔案中
   3. 而第 3 步驟其實做的事情就是修改 `~/.gitconfig` 檔案

## 準備工作

1. **同步進度**
   - 先執行 `git pull` 將其他人最新的進度同步進來本機
   - 在我們這個專案中，請執行 `git pull origin main` 來同步

2. **切換/建立分支**
   1. `git checkout <分支名稱>` 切換分支
   2. `git branch <分支名稱>` 建立分支

3. **分支命名範例**
   1. `feat/navbar`
   2. `docs/README`

4. **分支的定位與用途**
   1. 因為屬於小型三人的開發團隊，命名方面不須太過強求
   2. 目前規劃以開發者名字作為分支名稱
      1. `feat/{開發者名字}`
      2. `fix/{開發者名字}`
      3. `docs/{開發者名字}`

## 工作小工具
>
> 推薦一些在 VScode 寫 C 過程中，好用的套件

1. C/C++：最基本的 C 語言語法檢查、自動填入，以及編譯器
2. Github Copilot：（神器，這份 HackMD 也有一半是它幫忙寫的）AI 協助程式碼撰寫
3. Clang Format：自動排版，讓程式碼看起來更整齊

## 命名規則

1. **文件類型**（如 `README.md`, `DEV_TIPS.md`, `STRUCT.md`）：全大寫，單字間以 `_` 分隔
2. **程式碼檔案類型**（如 `main.c`, `struct.c`, `function.c`）：全小寫，單字間以 `_` 分隔
3. **變數名稱**：小寫開頭，單字間以大寫分隔（如 `int myAge = 20;`），又稱為小駝峰式命名法

## 提交訊息

> 撰寫 commit message 時須遵守 [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/)

```git
// 範例提交訊息 //

docs[README]: fix chown argument
feat[navbar]: Add i18n language switch
```

### 提交方式

1. `git add <檔案名稱>` 將檔案加入暫存區
2. `git commit` 提交暫存區的檔案
3. 此時會跳出 **Vim** 編輯器，請依照下方格式填寫 commit message

- 補充：如果不想使用 `vim` 編輯器怎麼辦？

### 格式概觀

 `<類別>` `[影響範圍（選填）]:` `<敘述>`

#### 1. 類別 （大部分情況下，只會用到前 5 種類別）

   | 類別 | 說明 |
   | --- | --- |
   | `feat` | 新增/修改功能 (feature) |
   | `fix` | 修補 bug (bug fix) |
   | `docs` | 文件 (documentation) |
   | `chore` | 建構程序或輔助工具的變動 (maintain) |
   | `style` | 格式 (不影響程式碼運行的變動 white-space, formatting, missing semi colons, etc) |
   | `refactor` | 重構 (既不是新增功能，也不是修補 bug 的程式碼變動) |
   | `perf` | 改善效能 (A code change that improves performance) |
   | `test` | 增加測試 (when adding missing tests) |
   | `revert` | 撤銷回覆先前的 commit 例如：revert: type(scope): subject (回覆版本：xxxx) |

- 關於每個類別的中文詳細說明：[📖 Git Message 這樣寫會更好](https://wadehuanglearning.blogspot.com/2019/05/commit-commit-commit-why-what-commit.html)
