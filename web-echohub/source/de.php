<?php
include('test.php');

//print_r($GLOBALS[O0]);
$file_en = file_get_contents("test.php");
foreach($GLOBALS[O0] as $index=>$symbol){
    //recover functions
    $file_en=preg_replace('/\$GLOBALS[\{|\[]O0[\}|\]][\{|\[]0x[0]*'.(string)dechex($index).'[\}|\]]\(/', $symbol."(", $file_en);
    $file_en=preg_replace('/\$GLOBALS[\{|\[]O0[\}|\]][\{|\[]'.(string)$index.'[\}|\]]\(/', $symbol."(", $file_en);
    $file_en=preg_replace('/\$(OO00O|O000O0|O0OO00|O0OOOO|O00OO0|O0000O|OOO00|OOOO0|O0O00|OO0O)[\{|\[]0x[0]*'.(string)dechex($index).'[\}|\]]\(/', $symbol."(", $file_en);
    $file_en=preg_replace('/\$(OO00O|O000O0|O0OO00|O0OOOO|O00OO0|O0000O|OOO00|OOOO0|O0O00|OO0O)[\{|\[]'.(string)$index.'[\}|\]]\(/', $symbol."(", $file_en);
    //recover strings
    $file_en=preg_replace('/\$GLOBALS[\{|\[]O0[\}|\]][\{|\[]0x[0]*'.(string)dechex($index).'[\}|\]]/', "'".addslashes($symbol)."'", $file_en);
    $file_en=preg_replace('/\$GLOBALS[\{|\[]O0[\}|\]][\{|\[]'.(string)$index.'[\}|\]]/', "'".addslashes($symbol)."'", $file_en);
    $file_en=preg_replace('/\$(OO00O|O000O0|O0OO00|O0OOOO|O00OO0|O0000O|OOO00|OOOO0|O0O00|OO0O)[\{|\[]0x[0]*'.(string)dechex($index).'[\}|\]]/', "'".addslashes($symbol)."'", $file_en);
    $file_en=preg_replace('/\$(OO00O|O000O0|O0OO00|O0OOOO|O00OO0|O0000O|OOO00|OOOO0|O0O00|OO0O)[\{|\[]'.(string)$index.'[\}|\]]/',"'".addslashes($symbol)."'", $file_en);
    
}                 //$GLOBALS    {        O0    }                 [   0x00000         2c                       ]

//we know $GLOBALS[0x0010] is "\x00"
$file_en = preg_replace('/\'\$[^\']*\'/','"\\x00"',$file_en);


file_put_contents('de_.php',$file_en);