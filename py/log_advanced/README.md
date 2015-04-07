###Configuring Logging

Programmers can configure logging in 3 ways:

1. Creating loggers, handlers, and formatters explicitly using Python code
that calls the configuration methods
2. Creating a logging config file and reading it using `fileConfig()` function
3. Creating a dictionary of configuration information and passing it to the
`dictConfig()` function

Formatter 常用信息

|formatter| 描述|
|:--------------------|:-------------------------------------|
|%(name)s | Logger的名字|
|%(levelno)s | 数字形式的日志级别|
|%(levelname)s | 文本形式的日志级别|
|%(pathname)s | 调用日志输出函数的模块的完整路径名，可能没有|
|%(filename)s | 调用日志输出函数的模块的文件名|
|%(module)s | 调用日志输出函数的模块名|
|%(funcName)s | 调用日志输出函数的函数名|
|%(lineno)d | 调用日志输出函数的语句所在的代码行|
|%(created)f | 当前时间，用UNIX标准的表示时间的浮 点数表示|
|%(relativeCreated)d | 输出日志信息时的，自Logger创建以 来的毫秒数|
|%(asctime)s | 字符串形式的当前时间。默认格式是 “2003-07-08 16:49:45,896”。逗号后面的是毫秒|
|%(thread)d | 线程ID。可能没有|
|%(threadName)s | 线程名。可能没有|
|%(process)d | 进程ID。可能没有|
|%(message)s | 用户输出的消息|

examples:

- [x] config logger by calling API in code
- [x] config with a dict in file
- [x] config with a traditional config file
- [ ] logger can rotate according to size
- [ ] logger can rotate according to time
- [x] logger rotate on restart
