/***************************************************************************
 * Evil EarthCloud's Insidious Bomb, Version 1.0
 * Copyright 2026, Evil EarthCloud Incorporated. All rights reserved.
 *
 * LICENSE:
 *
 * Evil EarthCloud Incorporated (the PERPETRATOR) hereby grants you (the
 * VICTIM) explicit permission to use this bomb (the BOMB).  This is a
 * time limited license, which expires on the death of the VICTIM.
 * The PERPETRATOR takes no responsibility for damage, frustration,
 * insanity, bug-eyes, carpal-tunnel syndrome, loss of sleep, or other
 * harm to the VICTIM.  Unless the PERPETRATOR wants to take credit,
 * that is.  The VICTIM may not distribute this bomb source code to
 * any enemies of the PERPETRATOR.  No VICTIM may debug,
 * reverse-engineer, run "strings" on, decompile, decrypt, or use any
 * other technique to gain knowledge of and defuse the BOMB.  BOMB
 * proof clothing may not be worn when handling this program.  The
 * PERPETRATOR will not apologize for the PERPETRATOR's poor sense of
 * humor.  This license is null and void where the BOMB is prohibited
 * by law.
 ***************************************************************************/

 /***************************************************************************
 * Evil EarthCloud的阴险炸弹，版本 1.0
 * 版权所有 2026，Evil EarthCloud股份有限公司。保留所有权利。
 *
 * 许可协议：
 *
 * Evil EarthCloud股份有限公司（以下简称“施暴者”）特此授予你（以下简称
 * “受害者”）使用此炸弹（以下简称“炸弹”）的明确许可。这是一份限时
 * 许可，将在“受害者”死亡时到期。“施暴者”不对“受害者”遭受的损害、
 * 挫败感、精神失常、瞪眼、腕管综合征、失眠或其他伤害承担任何责任。
 * 除非，“施暴者”想把这当成自己的功劳。“受害者”不得将此炸弹源代码
 * 分发给“施暴者”的任何敌人。任何“受害者”不得调试、逆向工程、
 * 运行“strings”命令、反编译、解密，或使用任何其他技术来获取知识并
 * 拆除炸弹。处理此程序时，不得穿戴防弹衣。“施暴者”不会为“施暴者”
 * 糟糕的幽默感道歉。在“炸弹”被法律禁止的地方，此许可无效。
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "support.h"
#include "phases.h"

/*
 * Note to self: Remember to erase this file so my victims will have no
 * idea what is going on, and so they will all blow up in a
 * spectaculary fiendish explosion. -- Evil EarthCloud
 */

 /*
 * 给自己的备注：记得删掉这个文件，这样我的受害者们就完全不知道
 * 发生了什么，他们就会在一场极其邪恶的壮观爆炸中全部炸飞。
 * —— Evil EarthCloud
 */

FILE *infile;

/* ------------------------------------------------------------------ */
/* Single-phase mode: run phase n alone, in a child process.           */
/*                                                                     */
/* The real phase code runs in a forked child, so a failure exits      */
/* through the usual explode_bomb() -> exit(8) path without taking     */
/* the parent (and the grader) down with it. The parent simply         */
/* reaps the child and reports PASS / FAILED.                          */
/*                                                                     */
/* This is what makes the five phases independent of one another:      */
/* a wrong phase 3 does not cost you phases 4 and 5.                   */
/* ------------------------------------------------------------------ */


/* ------------------------------------------------------------------ */
/* 单阶段模式：单独运行第 n 关，在一个子进程中。                        */
/*                                                                     */
/* 真正的关卡代码在一个 fork 出的子进程中运行，因此失败会通过通常的   */
/* explode_bomb() -> exit(8) 路径退出，而不会拖垮父进程（以及评分程序）。*/
/* 父进程只是回收子进程并报告 PASS / FAILED。                           */
/*                                                                     */
/* 这正是使五个关卡相互独立的原因：                                    */
/* 答错第 3 关不会导致你失去第 4 关和第 5 关。                          */
/* ------------------------------------------------------------------ */

static int run_one_phase(int n, char *input)
{
    pid_t pid;
    int status;

    fflush(stdout);
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(9);
    }

    if (pid == 0) {
        /* Child: stay quiet (explosions are swallowed here on purpose). */
        /* 子进程：保持安静（这里故意吞掉爆炸输出）。 */

        (void)freopen("/dev/null", "w", stdout);
        switch (n) {
        case 1: phase_1(input); break;
        case 2: phase_2(input); break;
        case 3: phase_3(input); break;
        case 4: phase_4(input); break;
        case 5: phase_5(input); break;
        default: exit(9);
        }
        exit(0);
    }

    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        exit(9);
    }

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        printf("[phase %d] PASS\n", n);
        fflush(stdout);
        return 0;
    }

    printf("[phase %d] FAILED\n", n);
    fflush(stdout);
    return 1;
}

int main(int argc, char *argv[])
{
    char *input;

    /* ------------------------------------------------------------------
     * Single-phase mode (used by the autograder; also handy for you when
     * you want to test one phase without replaying the whole chain):
     *
     *     ./bomb --phase 3            read one line from stdin, test phase 3
     *     ./bomb --phase 3 line3.txt  read one line from a file, test phase 3
     *
     * pass -> prints "[phase 3] PASS"   and exits 0
     * fail -> prints "[phase 3] FAILED" and exits 1
     * ------------------------------------------------------------------ */

    /* ------------------------------------------------------------------
     * 单阶段模式（供自动评分程序使用；当你想测试某一关而不必重玩整个
     * 流程时，对你也很方便）：
     *
     *     ./bomb --phase 3            从标准输入读取一行，测试第3关
     *     ./bomb --phase 3 line3.txt  从文件读取一行，测试第3关
     *
     * 通过 -> 打印 "[phase 3] PASS"   并以 0 退出
     * 失败 -> 打印 "[phase 3] FAILED" 并以 1 退出
    * ------------------------------------------------------------------ */

    if (argc >= 3 && strcmp(argv[1], "--phase") == 0) {
        int n = atoi(argv[2]);

        if (n < 1 || n > NUM_PHASES) {
            printf("Usage: %s --phase <1..%d> [<input_file>]\n", argv[0], NUM_PHASES);
            exit(2);
        }

        infile = stdin;
        if (argc >= 4) {
            if (!(infile = fopen(argv[3], "r"))) {
                printf("%s: Error: Couldn't open %s\n", argv[0], argv[3]);
                exit(8);
            }
        }

        initialize_bomb();
        input = read_line();
        return run_one_phase(n, input);
    }

    /* ------------------------------------------------------------------
     * Classic mode: the five phases chained together, first explosion
     * wins -- this is the original "defusing a binary bomb" experience.
     * ------------------------------------------------------------------ */

    /* When run with no arguments, the bomb reads its input lines
     * from standard input. */


    /* ------------------------------------------------------------------
     * 经典模式：五个阶段串联在一起，第一次爆炸就结束——
     * 这就是原汁原味的“拆解二进制炸弹”体验。
     * ------------------------------------------------------------------ */

    /* 当不带参数运行时，炸弹从标准输入读取其输入行。 */

    if (argc == 1) {
        infile = stdin;
    }

    /* When run with one argument <file>, the bomb reads from <file>
     * until EOF, and then switches to standard input. Thus, as you
     * defuse each phase, you can add its defusing string to <file> and
     * avoid having to retype it. */

     /* 当带一个参数 <file> 运行时，炸弹会从 <file> 读取内容，
     * 直到文件结束（EOF），然后切换到标准输入。因此，当你
     * 每拆除一个阶段，就可以把它的拆弹字符串添加到 <file> 中，
     * 免得重复输入。 */

    else if (argc == 2) {
        if (!(infile = fopen(argv[1], "r"))) {
            printf("%s: Error: Couldn't open %s\n", argv[0], argv[1]);
            exit(8);
        }
    }

    /* You can't call the bomb with more than 1 command line argument. */
    /* 你不能用多于 1 个命令行参数来调用炸弹程序。 */

    else {
        printf("Usage: %s [<input_file>]\n", argv[0]);
        printf("       %s --phase <1..%d> [<input_file>]\n", argv[0], NUM_PHASES);
        exit(8);
    }

    /* Do all sorts of secret stuff that makes the bomb harder to defuse. */
    /* 做各种秘密的事情，让炸弹更难被拆除。 */

    initialize_bomb();

    printf("Welcome to my fiendish little bomb. You have %d phases with\n", NUM_PHASES);
    printf("which to blow yourself up. Have a nice day!\n");

    input = read_line();  phase_1(input);  phase_defused();
    printf("Phase 1 defused. How about the next one?\n");

    /* The second phase is harder.  No one will ever figure out
     * how to defuse this... */
     /* 第二阶段更难。永远没人能弄明白
     * 该怎么拆除这个…… */

    input = read_line();  phase_2(input);  phase_defused();
    printf("That's number 2.  Keep going!\n");

    /* I guess this is too easy so far.  Some more complex code will
     * confuse people. */
     /* 我猜到目前为止这太简单了。再写些更复杂的代码，
     * 就能把人搞晕。 */

    input = read_line();  phase_3(input);  phase_defused();
    printf("Three down, two to go!\n");

    /* Oh yeah?  Well, how good is your math?  Try on this saucy problem! */
    /* 哦，是吗？那你的数学有多好？来试试这个刁钻的难题吧！ */

    input = read_line();  phase_4(input);  phase_defused();
    printf("So you got that one.  Try this one.\n");

    /* Round and 'round in memory we go, where we stop, the bomb blows! */
    /* 我们在内存里转了一圈又一圈，我们停在哪里，炸弹就在哪里爆炸！ */

    input = read_line();  phase_5(input);  phase_defused();

    return 0;
}
