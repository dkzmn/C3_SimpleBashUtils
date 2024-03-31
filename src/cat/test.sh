#! /bin/bash

make

mkdir -p testfiles
rm -rf testfiles/*

for i in {0..255}
do 
    if [ $(($i)) != 138 ]; then
        echo -en '\x'$(printf "%0x" $i)'' >> ./testfiles/1_all_bytes.txt
        if [ $(($i % 16)) == 0 ]; then
            echo -en '\x'$(printf "%0x" 10)'' >> ./testfiles/1_all_bytes.txt
        fi
    fi
done

for i in {0..9}
do 
    echo -en ' !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~\n' >> ./testfiles/long_strings.txt
done

echo -en '1234\t5678\t8765\t4321\n\n\n\t\t\t\t\t\t\t\t\t\t\n\n\n1234\t\t\t4321\n' >> ./testfiles/tabs.txt

echo -en 'АБВ\n123\nabc' >> ./testfiles/small.txt

echo -en 'POSIX (англ. Portable Operating System Interface — переносимый интерфейс операционных систем) — набор стандартов, описывающих интерфейсы между операционной системой и прикладной программой (системный API), библиотеку языка C и набор приложений и их интерфейсов. Стандарт создан для обеспечения совместимости различных UNIX-подобных операционных систем и переносимости прикладных программ на уровне исходного кода, но может быть использован и для не-Unix систем.\n\nСерия стандартов POSIX была разработана комитетом 1003 IEEE. Международная организация по стандартизации (ISO) совместно c Международной электротехнической комиссией (IEC) приняли стандарт POSIX под названием ISO/IEC 9945[2]. Версии стандарта POSIX являются основой соответствующих версий стандарта Single UNIX Specification. Стандарт POSIX определяет интерфейс операционной системы, а соответствие стандарту Single UNIX Specification определяет реализацию интерфейса и позволяет операционным системам использовать торговую марку UNIX[3].\n\nНазвание «POSIX» было предложено Ричардом Столлманом[4]. Введение в POSIX.1 гласит: «Ожидается произношение „позикс“ как в слове „позитив“, а не „посикс“. Произношение опубликовано в целях обнародования стандартного способа ссылки на стандартный интерфейс операционной системы». «POSIX» является зарегистрированным товарным знаком IEEE[4].' >> ./testfiles/posix.txt

cp s21_cat.c testfiles/c.txt

cat1="cat "
cat21="./s21_cat "

tests=0
ok=0
fail=0

for filename in testfiles/*.txt
do
    for i in {0..63}
    do
        flags=""
        if [ $(($i % 2)) == 1 ]; then
            flags+="-b ";
        fi
        i=$(($i/2))
        if [ $(($i % 2)) == 1 ]; then
            flags+="-e ";
        fi
        i=$(($i/2))
        if [ $(($i % 2)) == 1 ]; then
            flags+="-n ";
        fi
        i=$(($i/2))
        if [ $(($i % 2)) == 1 ]; then
            flags+="-s ";
        fi
        i=$(($i/2))
        if [ $(($i % 2)) == 1 ]; then
            flags+="-t ";
        fi
        i=$(($i/2))
        if [ $(($i % 2)) == 1 ]; then
            flags+="-v ";
        fi
        i=$(($i/2))
	    # if [ $(($i % 2)) == 1 ]; then
        #     flags+="--number-nonblank ";
        # fi
        # i=$(($i/2))
        # if [ $(($i % 2)) == 1 ]; then
        #     flags+="--number ";
        # fi
        # i=$(($i/2))
        # if [ $(($i % 2)) == 1 ]; then
        #     flags+="--squeeze-blank ";
        # fi
        # i=$(($i/2))

        files=($(find ./testfiles -type f))
        n=${#files[@]}
        r1=$(($RANDOM % $n))
        r2=$(($RANDOM % $n))
    
        command="diff <(${cat1}${flags}${filename}) <(${cat21}${flags}${filename})"
        output=$(eval "$command")
        if [ "$output" = "" ]; then
            echo "TEST OK $command"
        ok=$(($ok+1))
        else
            echo "TEST FAIL $command"
        fail=$(($fail+1))
        fi
        tests=$(($tests+1))
        command="diff <(${cat1}${flags}${filename} ${files[$r1]}) <(${cat21}${flags}${filename} ${files[$r1]})"
        output=$(eval "$command")
        if [ "$output" = "" ]; then
            echo "TEST OK $command"
            ok=$(($ok+1))
        else
            echo "TEST FAIL $command"
            fail=$(($fail+1))
        fi
        tests=$(($tests+1))
        # command="diff <(${cat1}${flags}${filename} ${files[$r1]} ${files[$r2]}) <(${cat21}${flags}${filename} ${files[$r1]} ${files[$r2]})"
        # output=$(eval "$command")
        # if [ "$output" = "" ]; then
        #     echo "TEST OK $command"
        #     ok=$(($ok+1))
        # else
        #     echo "TEST FAIL $command"
        #     fail=$(($fail+1))
        # fi
        # tests=$(($tests+1))
    done
done
echo -en "----------\nTESTS: $tests\nFAILED: $fail\nOK: $ok\n"
