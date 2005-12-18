typedef union
{
    int pos;
    int ival;
    double fval;
    c_str sval;
    
    a_Program program;
    a_Section program_section;
    a_Stmt_List stmt_list;
    a_Class_Def class_def;
    a_Class_Ext class_ext;
    a_Class_Body class_body;
    a_Stmt stmt;
    a_Exp exp;
    a_Func_Def func_def;
    a_Var_Decl_List var_decl_list;
    a_Var_Decl var_decl;
    a_Type_Decl type_decl;
    a_Arg_List arg_list;
    a_Id_List id_list;
    a_Array_Sub array_sub;
} YYSTYPE;
#define	ID	257
#define	STRING_LIT	258
#define	NUM	259
#define	FLOAT	260
#define	POUND	261
#define	COMMA	262
#define	COLON	263
#define	SEMICOLON	264
#define	LPAREN	265
#define	RPAREN	266
#define	LBRACK	267
#define	RBRACK	268
#define	LBRACE	269
#define	RBRACE	270
#define	DOT	271
#define	PLUS	272
#define	MINUS	273
#define	TIMES	274
#define	DIVIDE	275
#define	PERCENT	276
#define	EQ	277
#define	NEQ	278
#define	LT	279
#define	LE	280
#define	GT	281
#define	GE	282
#define	AND	283
#define	OR	284
#define	ASSIGN	285
#define	IF	286
#define	THEN	287
#define	ELSE	288
#define	WHILE	289
#define	FOR	290
#define	DO	291
#define	LOOP	292
#define	BREAK	293
#define	CONTINUE	294
#define	NULL_TOK	295
#define	FUNCTION	296
#define	RETURN	297
#define	QUESTION	298
#define	EXCLAMATION	299
#define	S_OR	300
#define	S_AND	301
#define	S_XOR	302
#define	PLUSPLUS	303
#define	MINUSMINUS	304
#define	DOLLAR	305
#define	SIMULT	306
#define	PATTERN	307
#define	CODE	308
#define	TRANSPORT	309
#define	HOST	310
#define	TIME	311
#define	WHENEVER	312
#define	NEXT	313
#define	UNTIL	314
#define	EVERY	315
#define	BEFORE	316
#define	AFTER	317
#define	AT	318
#define	AT_SYM	319
#define	ATAT_SYM	320
#define	NEW	321
#define	SIZEOF	322
#define	TYPEOF	323
#define	SAME	324
#define	PLUS_CHUCK	325
#define	MINUS_CHUCK	326
#define	TIMES_CHUCK	327
#define	DIVIDE_CHUCK	328
#define	S_AND_CHUCK	329
#define	S_OR_CHUCK	330
#define	S_XOR_CHUCK	331
#define	SHIFT_RIGHT_CHUCK	332
#define	SHIFT_LEFT_CHUCK	333
#define	PERCENT_CHUCK	334
#define	SHIFT_RIGHT	335
#define	SHIFT_LEFT	336
#define	TILDA	337
#define	CHUCK	338
#define	COLONCOLON	339
#define	S_CHUCK	340
#define	AT_CHUCK	341
#define	LEFT_S_CHUCK	342
#define	UNCHUCK	343
#define	CLASS	344
#define	INTERFACE	345
#define	EXTENDS	346
#define	IMPLEMENTS	347
#define	PUBLIC	348
#define	PROTECTED	349
#define	PRIVATE	350
#define	STATIC	351
#define	ABSTRACT	352
#define	CONST	353
#define	SPORK	354
#define	L_HACK	355
#define	R_HACK	356


extern YYSTYPE yylval;
