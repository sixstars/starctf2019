package fano
import "fmt"
import "math"
import "github.com/imroc/biu"

const N = 0x100

type Character struct{
	name string //字符
	code string //编码
	freq int //出现频率
}

type Fano struct {
	codebook map[uint8](*Character)
	queue []Character
}

func (f *Fano) Fano_init(file_content string){
	f.codebook = make(map[uint8]*Character)
	f.queue = make([]Character,N)
    for i:=0; i<N; i++{
     	C := new(Character)
     	C.name = string(i)
     	C.code = ""
     	C.freq = 1
     	f.queue[uint8(i)] = *C
     	f.codebook[uint8(i)] = C
    }
    
    f.timesOfChars(file_content)
    f.fano_sort()
    f.fano_generate(0, N-1)
}

func (f *Fano) fano_sort() {
	// 逆序的插入排序, 按照字符频率由高到低排列;
    for i := 0; i < N-1; i++ {
    	j:= i+1; v:=f.queue[j]
        for ;  j >= 1 && v.freq > f.queue[j-1].freq; j-- {
          f.queue[j] = f.queue[j-1]
        }
        f.queue[j]=v
      }
}

func (f *Fano) timesOfChars(file_content string) {
    for _, c := range file_content {
    	f.codebook[uint8(c)].freq +=1
    }
	                           
}

func (f *Fano) fano_generate(a int, b int){
	if b-a<1 {
		return
	}
	sum := 0
	for i:=a; i<=b; i++ {
		sum += f.codebook[uint8(i)].freq//charNum[i]
	}
	pa :=0
	var s []int
	s = make([]int, N)
    for i:=a; i<=b; i++ {
    	pa +=  f.codebook[uint8(i)].freq
    	s[i] = int(math.Abs(float64(2*pa -sum)))
    }
    min := a
    for i:=a+1; i<=b; i++{
    	if s[i] <= s[min]{
    		min=i
    	}
    }
    for i :=a; i<=b; i++ {
    	if i<=min{
    		f.codebook[uint8(i)].code +="0"
    	}else{
    		f.codebook[uint8(i)].code +="1"
    	}

    }
    f.fano_generate(a, min)
    f.fano_generate(min+1, b)
}

func Bytes2Str(Bytes string) string{
	cipher:=""
	for _,b := range Bytes{
		cipher += biu.ToBinaryString(byte(b))
	}
    return cipher
}

func (f *Fano) Decode(Bytes string) string {
	cipher := Bytes2Str(Bytes)
    p := 0
    plain := ""
    for i:=1; i<=len(cipher); i++ {
     	tmp := cipher[p:i]
     	for j:=0; j<N; j++ {
     		if tmp == f.codebook[uint8(j)].code {
     			plain += string(j)
     			p=i
     			break
     		}
     	}
    }
    return plain
}

func Str2Bytes(cipher string) string{
	for len(cipher)%8 != 0{
		cipher += "0"
	}
    Bytes := ""
	for p:=0; p<len(cipher); p+=8{
        var a byte
	    biu.ReadBinaryString(cipher[p:p+8], &a)
	    Bytes+=string(a)

	}
    // for _,x :=range Bytes{
    // 	fmt.Println(byte(x))
    // }
	return Bytes

}

func (f *Fano) Encode(plain string) string {
    cipher := ""
    for  i:=0; i< len(plain); i++{
      cipher += f.codebook[uint8(plain[i])].code
    }
    return Str2Bytes(cipher)
}

func main(){
	fmt.Println("hello world!")
}
