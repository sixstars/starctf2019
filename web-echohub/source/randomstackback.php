<?php


require_once 'sandbox.php';


$seed = time();
srand($seed);
define("INS_OFFSET",rand(0x0000,0xffff));

$regs = array(
    'eax'=>0x0,
    'ebp'=>0x0,
    'esp'=>0x0,
    'eip'=>0x0,
);


function aslr(&$value,$key)
{
    $value = $value + 0x60000000 + INS_OFFSET + 1 ;
}
$func_ = array_flip($func);
array_walk($func_,"aslr");
$plt = array_flip($func_);


function handle_data($data){
    $data_len = strlen($data);
    $bytes4_size = $data_len/4+(1*($data_len%4));
    $cut_data = str_split($data,4);
    $cut_data[$bytes4_size-1] = str_pad($cut_data[$bytes4_size-1],4,"\x00");
    foreach ($cut_data as $key=>&$value){
        $value = strrev(bin2hex($value));
    }
    return $cut_data;
}

function gen_canary(){
    $chars = 'abcdefghijklmnopqrstuvwxyzABCDEFGHJKLMNPQEST123456789';
    $c_1 = $chars[rand(0,strlen($chars)-1)];
    $c_2 = $chars[rand(0,strlen($chars)-1)];
    $c_3 = $chars[rand(0,strlen($chars)-1)];
    $c_4 = "\x00";
    return handle_data($c_1.$c_2.$c_3.$c_4)[0];
}
$canary = gen_canary();
$canarycheck = $canary;

function check_canary(){
    global $canary;
    global $canarycheck;
    if($canary != $canarycheck){
        die("emmmmmm...Don't attack me!");
    }

}

Class stack{
    private $ebp,$stack,$esp;

    public function __construct($retaddr,$data) {
        $this->stack = array();
        global $regs;
        $this->ebp = &$regs['ebp'];
        $this->esp = &$regs['esp'];
        $this->ebp = 0xfffe0000 + rand(0x0000,0xffff);
        global $canary;
        $this->stack[$this->ebp - 0x4] = &$canary;
        $this->stack[$this->ebp] = $this->ebp + rand(0x0000,0xffff);
        $this->esp = $this->ebp - (rand(0x20,0x60)*4);
        $this->stack[$this->ebp + 0x4] = dechex($retaddr);
        if($data != NULL)
            $this->pushdata($data);
    }

    public function pushdata($data){
        $data = handle_data($data);
        for($i=0;$i<count($data);$i++){
            $this->stack[$this->esp+($i*4)] = $data[$i];//no args in my stack haha
            check_canary();
        }
    }

    public function recover_data($data){

        return hex2bin(strrev($data));

    }


    public function outputdata(){
        global $regs;
        echo "root says: ";
        while(1){
            if($this->esp == $this->ebp-0x4)
                break;
            $this->pop("eax");
            $data = $this->recover_data($regs["eax"]);
            $tmp = explode("\x00",$data);
            echo $tmp[0];
            if(count($tmp)>1){
                break;
            }
        }

    }
    public function ret(){

        $this->esp = $this->ebp;
        $this->pop('ebp');
        $this->pop("eip");
        $this->call();
    }

    public function get_data_from_reg($regname){
        global $regs;
        $data = $this->recover_data($regs[$regname]);
        $tmp = explode("\x00",$data);
        return $tmp[0];
    }

    public function call()
    {
        global $regs;
        global $plt;
        $funcaddr = hexdec($regs['eip']);
        if(isset($_REQUEST[$funcaddr])) {
            $this->pop('eax');
            $argnum = (int)$this->get_data_from_reg("eax");
            $args = array();
            for($i=0;$i<$argnum;$i++){
                $this->pop('eax');
                $argaddr = $this->get_data_from_reg("eax");
                array_push($args,$_REQUEST[$argaddr]);
            }
            call_user_func_array($plt[$funcaddr],$args);
        }
        else
        {
            call_user_func($plt[$funcaddr]);
        }

    }

    public function push($reg){
        global $regs;
        $reg_data = $regs[$reg];
        if( hex2bin(strrev($reg_data)) == NULL ) die("data error");
        $this->stack[$this->esp] = $reg_data;
        $this->esp -= 4;
    }

    public function pop($reg){
        global $regs;
        $regs[$reg] = $this->stack[$this->esp];
        $this->esp += 4;

    }

    public function __call($_a1,$_a2)
    {
        check_canary();
    }

}

if(isset($_POST['data'])) {
        $phpinfo_addr = array_search('phpinfo', $plt);
        $gets = $_POST['data'];
        $main_stack = new stack($phpinfo_addr, $gets);
        echo "--------------------output---------------------</br></br>";
        $main_stack->outputdata();
        echo "</br></br>------------------phpinfo()------------------</br>";
        $main_stack->ret();
}
