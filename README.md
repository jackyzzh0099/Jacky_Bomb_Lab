# Lingrui 炸弹实验室

*改编自 CS:APP Bomb Lab（卡内基梅隆大学）。由 Evil EarthCloud 担任反派。*

**邪恶地球云会把你们炸上天**

**注意：** 本实验故意使用英文。

---

## 1. 引言

阴险的 **Evil EarthCloud** 在你的机器上植入了一枚二进制炸弹。

他说：“Hina Youmiya 是有史以来最伟大的女声优。如果你不同意，我就炸掉你的电脑。”

真是个疯子。你最好拆除炸弹——否则你的电脑就会被毁掉。

*二进制炸弹*是一个由五个阶段序列组成的程序。每个阶段都要求你在 `stdin` 上输入一个特定字符串。如果你输入了正确的字符串，该阶段就被**拆除**，炸弹进入下一阶段。否则，炸弹会打印 `BOOM!!!` 然后终止，从而爆炸。当每个阶段都被拆除时，炸弹就被拆除了，此时它会打印：

```text
Congratulations! You've defused the bomb!
```

**你的任务，尽管你别无选择只能接受，就是在截止日期前拆除你的炸弹。** 祝你好运，欢迎加入拆弹小组。

> ⚠️ **警告：不要试图通过暴力破解来拆除炸弹。** 这枚炸弹并非为暴力破解而设计，你的 CPU 周期还有更好的用途。你应该*阅读汇编*并使用*调试器*。

---

## 2. 安排

### 2.1 你会得到什么

| 文件                                | 描述                                    |
| --------------------------------- | ------------------------------------- |
| `bomb`                            | 炸弹本体：一个 x86-64 Linux 可执行文件。**不要修改它。** |
| `bomb.c`                          | 你唯一获得的源代码——只有 `main`，**没有**阶段逻辑。      |
| `solution.txt`                    | 你的答案放在这里：**每个阶段一行，共五行**。              |
| `bomb.sha256`                     | 评分器用来检测二进制是否被篡改的校验和。                  |
| `tools/autograde.sh`              | 自动评分器。CI 运行的正是这个脚本。                   |
| `.github/workflows/autograde.yml` | 触发自动评分器的 GitHub Actions 工作流。          |

> 安装说明、调试指南和阶段提示位于作业文档中。

### 2.2 如何提交

1. 使用此模板创建你自己的仓库（**Use this template**），然后把答案放进 `solution.txt`，提交并推送。
2. 打开 **Actions** 选项卡 → 选择 `Autograde` → **Run workflow**。
3. 打开已完成的运行：评分报告位于底部的 **Summary** 部分（也可作为 `autograde-report` 工件获取）。

你也应该在本地运行评分器——它只需一秒钟：

```bash
bash tools/autograde.sh              # 评分 solution.txt
bash tools/autograde.sh other.txt    # 评分另一个文件
```

---

## 3. 开始

炸弹是一个 **Linux x86-64 ELF 可执行文件**：你需要 Linux，或者 Windows + **WSL2**。

```bash
chmod +x bomb

# (1) 输入一些乱七八糟的内容，看它爆炸——对炸弹产生一点敬畏
./bomb
> test
...
BOOM!!!
The bomb has blown up.

# (2) 给它喂一个文件：炸弹从文件读取行，然后回退到 stdin。
#     这就是你的工作方式：每拆除一个阶段，就把它的答案追加到文件中。
./bomb solution.txt
```

`solution.txt` 一开始包含五行 `???` 占位符。每拆除一个阶段就替换一行——这样你就不必重新输入之前的答案。

### 3.1 单阶段模式

炸弹也可以**单独运行一个阶段**，当你卡在某个特定阶段时（或者想在不重放整个链的情况下检查答案时），这会很方便：

```bash
./bomb --phase 3              # 从 stdin 读取一行，只测试第 3 阶段
./bomb --phase 3 line3.txt    # ... 或者从文件读取
# pass -> [phase 3] PASS   (exit code 0)
# fail -> [phase 3] FAILED (exit code 1)
```

经典链式模式不变：`./bomb solution.txt` 按顺序运行全部五个阶段，并在第一次爆炸时终止。

---

## 4. 评分

| 项目     | 规则                                                       |
| ------ | -------------------------------------------------------- |
| 分值     | **每阶段 20 分**，共 100 分                                     |
| 分阶段评分  | 每个阶段**单独评分**（`./bomb --phase N`）。第 3 阶段失败不会影响第 4 和第 5 阶段 |
| 二进制完整性 | 评分前会用 `bomb.sha256` 对 `bomb` 进行校验。修改过的二进制得 **0 分**       |
| 报告     | 一个每阶段 ✅/❌ 表格，写入 Actions Summary 并作为工件上传                  |

在所有五个阶段都被拆除之前，评分器会让 CI 运行失败（红色 ❌）。部分解决的炸弹仍会生成报告，准确显示哪些阶段已完成。

---

## 5. 参考

- *Computer Systems: A Programmer's Perspective*（CS:APP）第 3 章
- CS:APP Bomb Lab 实验说明（卡内基梅隆大学） 

---















# Lingrui Bomb Lab

*Adapted from the CS:APP Bomb Lab (Carnegie Mellon University). Featuring Evil EarthCloud as the villain.*

**THE EVIL EARTHCLOUD WILL BLOW YOU GUYS UP**

**Note:** this lab is in English on purpose.

## 1. Introduction

The nefarious **Evil EarthCloud** has planted a binary bomb on your machine.

He says, "Hina Youmiya is the greatest female voice actor ever. If you don't agree, I will blow up your computer."

What a crazy guy. You'd better defuse the bomb — or your computer will be destroyed.

A *binary bomb* is a program that consists of a sequence of **five phases**. Each phase expects you
to type a particular string on `stdin`. If you type the correct string, then the phase is **defused**
and the bomb proceeds to the next phase. Otherwise, the bomb **explodes** by printing `BOOM!!!` and
then terminating. The bomb is defused when every phase has been defused, at which point it prints:

```text
Congratulations! You've defused the bomb!
```

**Your mission, which you have no choice but to accept, is to defuse your bomb before the due date.**
Good luck, and welcome to the bomb squad.

> ⚠️ **Warning: do not try to defuse the bomb by brute force.** The bomb is not designed to be
> brute-forced, and you have better things to do with your CPU cycles. You are expected to *read the
> assembly* and to *use a debugger*.

---

## 2. Logistics

### 2.1 What you are given

| File                              | Description                                                          |
| --------------------------------- | -------------------------------------------------------------------- |
| `bomb`                            | The bomb itself: an x86-64 Linux executable. **Do not modify it.**   |
| `bomb.c`                          | The only source code you get — just `main`, with **no** phase logic. |
| `solution.txt`                    | Where your answers go: **one line per phase, five lines total**.     |
| `bomb.sha256`                     | Checksum used by the grader to detect a tampered binary.             |
| `tools/autograde.sh`              | The autograder. The CI runs exactly this script.                     |
| `.github/workflows/autograde.yml` | GitHub Actions workflow that triggers the autograder.                |

> Setup instructions, a debugging guide and phase hints live in the assignment document.

### 2.2 How to hand in

1. Create your own repository from this template (**Use this template**), then put your
   answers in `solution.txt`, commit, and push.
2. Open the **Actions** tab → select `Autograde` → **Run workflow**.
3. Open the finished run: the grading report is in the **Summary** section at the bottom
   (and also available as the `autograde-report` artifact).

You should also run the grader locally — it takes a second:

```bash
bash tools/autograde.sh              # grades solution.txt
bash tools/autograde.sh other.txt    # grades another file
```

---

## 3. Getting Started

The bomb is a **Linux x86-64 ELF executable**: you need Linux, or Windows + **WSL2**.

```bash
chmod +x bomb

# (1) Type something silly and watch it blow up — build some respect for the bomb
./bomb
> test
...
BOOM!!!
The bomb has blown up.

# (2) Feed it a file: the bomb reads lines from the file, then falls back to stdin.
#     This is how you work: as you defuse each phase, append its answer to the file.
./bomb solution.txt
```

`solution.txt` starts out with five `???` placeholder lines. Replace them one by one as you defuse
each phase — that way you never have to retype the earlier answers.

### 3.1 Single-phase mode

The bomb can also run **one phase in isolation**, which is convenient when you are stuck on a
particular phase (or when you want to check your answer without replaying the whole chain):

```bash
./bomb --phase 3              # read one line from stdin, test phase 3 only
./bomb --phase 3 line3.txt    # ... or read it from a file
# pass -> [phase 3] PASS   (exit code 0)
# fail -> [phase 3] FAILED (exit code 1)
```

The classic chained mode is unchanged: `./bomb solution.txt` runs all five phases in sequence and
terminates at the first explosion.

---

## 4. Grading

| Item              | Rule                                                                                                          |
| ----------------- | ------------------------------------------------------------------------------------------------------------- |
| Points            | **20 points per phase**, 100 total                                                                            |
| Per-phase grading | Each phase is graded **on its own** (`./bomb --phase N`). Failing phase 3 costs you nothing on phases 4 and 5 |
| Binary integrity  | `bomb` is checksummed (`bomb.sha256`) before grading. A modified binary scores **0**                          |
| Report            | A per-phase ✅/❌ table, written to the Actions Summary and uploaded as an artifact                             |

The grader fails the CI run (red ❌) until all five phases are defused. A partially solved bomb
still produces a report showing exactly which phases are done.

---

## 5. References

- *Computer Systems: A Programmer's Perspective* (CS:APP), Chapter 3
- The CS:APP Bomb Lab writeup (Carnegie Mellon University)