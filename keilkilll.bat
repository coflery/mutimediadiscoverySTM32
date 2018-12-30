rd /q /S MDK-ARM\DebugConfig
rd /q /S MDK-ARM\RTE
del *.i /S
del *.bak /S
del *.ddk /S
del *.edk /S
del *.lst /S
del *.lnp /S
del *.mpf /S
del *.mpj /S
del *.obj /S
del *.omf /S
::del *.opt /S  ::不允许删除JLINK的设置
del *.plg /S
del *.rpt /S
del *.tmp /S
del *.__i /S
del *.crf /S
del *.o /S
del *.d /S
del *.axf /S
del *.tra /S
del *.dep /S           
del JLinkLog.txt /S
del *.uvguix.*   /S

del *.iex /S
del *.htm /S
del *.sct /S
del *.map /S
::exit
