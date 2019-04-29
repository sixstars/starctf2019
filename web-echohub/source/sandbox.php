<?php
$banner = <<<EOF
<!--/?source=1-->
<pre>
 .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  
| .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. | 
| |  _________   | || |     ______   | || |  ____  ____  | || |     ____     | || |  ____  ____  | || | _____  _____ | || |   ______     | | 
| | |_   ___  |  | || |   .' ___  |  | || | |_   ||   _| | || |   .'    `.   | || | |_   ||   _| | || ||_   _||_   _|| || |  |_   _ \    | | 
| |   | |_  \_|  | || |  / .'   \_|  | || |   | |__| |   | || |  /  .--.  \  | || |   | |__| |   | || |  | |    | |  | || |    | |_) |   | | 
| |   |  _|  _   | || |  | |         | || |   |  __  |   | || |  | |    | |  | || |   |  __  |   | || |  | '    ' |  | || |    |  __'.   | | 
| |  _| |___/ |  | || |  \ `.___.'\  | || |  _| |  | |_  | || |  \  `--'  /  | || |  _| |  | |_  | || |   \ `--' /   | || |   _| |__) |  | | 
| | |_________|  | || |   `._____.'  | || | |____||____| | || |   `.____.'   | || | |____||____| | || |    `.__.'    | || |  |_______/   | | 
| |              | || |              | || |              | || |              | || |              | || |              | || |              | | 
| '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' | 
 '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  
 
 Welcome to random stack ! Try to execute `/readflag` :P
 
 </pre>

 <form action="/" method="post">root > <input name="data" placeholder="input some data"></form>
EOF;
echo $banner;
if(isset($_GET['source'])){
    $file = fopen("index.php","r");
    $contents = fread($file,filesize("index.php"));
    echo "---------------sourcecode---------------";
    echo base64_encode($contents);
    echo "----------------------------------------";
    fclose($file);
    highlight_file(__FILE__);

}
$disable_functions = ini_get("disable_functions");
$loadext = get_loaded_extensions();
foreach ($loadext as $ext) {
    if(in_array($ext,array("Core","date","libxml","pcre","zlib","filter","hash","sqlite3","zip"))) continue;
    else {
        if(count(get_extension_funcs($ext)?get_extension_funcs($ext):array()) >= 1)
            $dfunc = join(',',get_extension_funcs($ext));
        else
            continue;
        $disable_functions = $disable_functions.$dfunc.",";

    }
}
$func = get_defined_functions()["internal"];
foreach ($func as $f){
    if(stripos($f,"file") !== false || stripos($f,"open") !== false || stripos($f,"read") !== false || stripos($f,"write") !== false){
        $disable_functions = $disable_functions.$f.",";
    }
}

ini_set("disable_functions", $disable_functions);
//ini_set("open_basedir","/var/www/html/:/tmp/".md5($_SERVER['REMOTE_ADDR'])."/");