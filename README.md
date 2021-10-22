# Hirumeshimae

git使ったない人向けのサイト
https://backlog.com/ja/git-tutorial/

以下簡単な説明

作業ディレクトリの移動

```
$ cd Hirumeshimae
```

webapp_team4ディレクトリ上で

```
$ git branch
> * main
```
となっていることを確認する

```
fatal: not a git repository (or any of the parent directories): .git
```
のエラーが出る場合、下のエラーが出る場合を参照


ローカル状態を最新にする

```
$ git pull
```

作業ブランチを作成する

```
$ git branch 適当なブランチ名
```

ブランチが作成できているかを確認

```
$ git branch
> 適当なブランチ名
> * main
```

ブランチの切り替え

```
$ git checkout 適当なブランチ名
> Switched to branch '適当なブランチ名'
```
「適当なブランチ名」ブランチにファイルを追加

```
$ git add sample.txt
```

addしたファイルの確認

```
$ git status
```

コミットする

```
$ git commit -m "適当なコメント"
```

リモートリポジトリに反映させる

```
$ git push origin 適当なブランチ名
```
