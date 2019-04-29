### quicksort
#### note
In the problem discription, "I'm very quick", I mean the algrithom, not myself.
#### idea
Tell me the array size and numbers one by one, and I'll sort it by quicksort. The vulnerability is that we read number by gets() which is eary to find. The canary is open but you can bypass it by fill sth in  `__stk_check_fil` func on got table, like  `ret`.  We can overflow the variable first, and be careful  with the got and stack variable
#### solve
`hack.py`