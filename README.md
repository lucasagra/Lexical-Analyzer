# Preditive Recursive Lexical Analizer for Compilers Course

Grammar: \
 \
Pqm = Lsent ';' Sent\
Pqm = Sent\
Sent = 'if' Eb 'then' Lsent 'end'\
Sent = 'if' Eb 'then' Lsent 'else' Lsent 'end'\
Sent = 'for' Atr 'to' Ea 'repeat' Lsent 'end'\
Atr = 'id' '=' Expr\
Expr = Eb\
Eb = Eb 'boolop' Tb\
Eb = Tb\
Tb = 'not' Tb\
Tb = 'true'\
Tb = 'false'\
Tb = Ea\
Ea = Ea 'aritop' Ta\
Ea = Ta\
Ta = Ta 'multop' Fa\
Ta = Fa\
Fa = '(' Expr ')'\
Fa = 'id'\
Fa = 'cteN'\
