package main
import (
    "fano"
    "fmt"
    "bufio"
    "io/ioutil"
    "os"
)

/*
#include<stdio.h>
void init(){
   setbuf(stdin, 0LL);
   setbuf(stdout, 0LL);
   setbuf(stderr, 0LL);
}
*/
import "C"

const(
    flag = "./flag"
    essay = "If you cannot read all your books...fondle them---peer into them, let them fall open where they will, read from the first sentence that arrests the eye, set them back on the shelves with your own hands, arrange them on your own plan so that you at least know where they are. Let them be your friends; let them, at any rate, be your acquaintances."
    corpus = "./corpus.txt"
)

func check(result string)(bool){
    return result == essay
}


func main(){
    C.init()

    f, err := ioutil.ReadFile("corpus.txt")
    if err != nil {
        fmt.Print(err)
    }
    file_content := string(f)

    F := fano.Fano{}
    F.Fano_init(file_content)

    // // 将对应密文写入文件中
    // cipher_result := F.Encode(essay)
    // fmt.Println(cipher_result)

    // f1, err1 := os.Create("key.txt")
    // if err1 != nil {
    //     fmt.Print(err1)
    // }
    // defer f1.Close()

    // n, _ := f1.WriteString(cipher_result)
    // fmt.Printf("wrote %d bytes\n", n)


    fmt.Println("Say something:")
    var cipher string
    var buf []byte
    var n int
    buf = make([]byte, 500)
    // fmt.Scanln(&cipher)
    inputReader := bufio.NewReader(os.Stdin)
    // cipher, err = inputReader.ReadString()
    n, err = inputReader.Read(buf)
    cipher = string(buf)[:n]

    plain := F.Decode(cipher)

    if check(plain) {
        b, err := ioutil.ReadFile(flag)
        if err != nil {
            fmt.Print(err)
        }
        flag_content := string(b)
        fmt.Println(flag_content)
    }
    return
}