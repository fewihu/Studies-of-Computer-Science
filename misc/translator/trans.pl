% PVL Künstliche Intelligenz
% 18.05.2021

% ==============================================================================================
% deutsche Grammatik
satz(s(NP, VP)) --> nominal_phrase(NP, nom, Numerus, s), verbal_phrase(VP, Numerus).

% Nominalphrase:
% Parameter: (Baum, Kasus, Numerus, Eignung als Subjekt/Objekt)

% als Subjekt/Objekt mit einem Namen als Nomen (nur Singular)
nominal_phrase(np(N),_,s,s)	
	--> name(N,_).

% nur als Objekt (Nomen im Plural)
nominal_phrase(np(N), _, p, o) 			
	-->	nomen(N, _, p).

% nur als Objekt (Adjektiv und Nomen im Plural)
nominal_phrase(np(Adj,N), Kasus, p, o) 	
	-->	adjektiv(Adj, Genus, Kasus, p, u), 
    	nomen(N, Genus, p).

% als Subjekt oder Objekt
nominal_phrase(np(A, N), Kasus, Numerus, s)		
	-->	artikel(A, Genus, Kasus, Numerus), 
    	nomen(N, Genus, Numerus).

nominal_phrase(np(artb,Adj,N), Kasus, Numerus, s)	
	-->	artikel(artb, Genus, Kasus, Numerus), 
    	adjektiv(Adj, Genus, Kasus, Numerus, b), 
    	nomen(N, Genus, Numerus).

nominal_phrase(np(artu,Adj,N), Kasus, Numerus, s)
	--> artikel(artu, Genus, Kasus, Numerus),
    	adjektiv(Adj, Genus, Kasus, Numerus, u),
    	nomen(N, Genus, Numerus).

verbal_phrase(vp(V), Numerus) --> verb_intransitiv(V,Numerus).
verbal_phrase(vp(V, NP), Numerus) --> verb_transitiv(V, Kasus, Numerus), nominal_phrase(NP, Kasus, _,_).

% Artikel Nominativ
artikel(artb, m, nom, s) --> [A], {A = der}.
artikel(artb, f, nom, s) --> [A], {A = die}.
artikel(artb, n, nom, s) --> [A], {A = das}.

artikel(artu, m, nom, s) --> [A], {A = ein}.
artikel(artu, f, nom, s) --> [A], {A = eine}.
artikel(artu, f, nom, s) --> [A], {A = ein}.

artikel(artb, _, nom, p) --> [A], {A = die}.

% Artikel Akkusativ
artikel(artb, m, akk, s) --> [A], {A = den}.
artikel(artb, f, akk, s) --> [A], {A = die}.
artikel(artb, n, akk, s) --> [A], {A = das}.

artikel(artu, m, akk, s) --> [A], {A = einen}.
artikel(artu, f, akk, s) --> [A], {A = eine}.
artikel(artu, n, akk, s) --> [A], {A = ein}.

artikel(artb, _, akk, p) --> [A], {A = die}.

% Verben
verb_intransitiv(verb(V),Numerus) --> [V], {dict_verb(V,_,Numerus,i)}.
verb_transitiv(verb(V),akk,Numerus) --> [V], {dict_verb(V,_,Numerus,t)}.

% Nomen
nomen(nomen(N,Numerus),Genus,Numerus) 
	--> [N], {dict_nomen(N,Genus,_,_,Numerus)}.

name(nomen(N,s), s) --> [N], dict_name(nomen(N), _).

% Adjektive (tree, Genus, Kasus, Numerus, Bestimmheit) Verkettung von Stamm und Endung
% Bestimmtheit: u - Verwendung ohne Artikel oder mit unbestimmten Artikel
% 				b - Verwendung mit bestimmten Artikel

adjektiv(adj(Stamm), _, nom, s, b) 
	--> [Adj], {adjektiv_stamm(Stamm), Endung = e, concat(Stamm,Endung,Adj)}.
adjektiv(adj(Stamm), _, nom, p, b)
	--> [Adj], {adjektiv_stamm(Stamm), Endung = en,concat(Stamm,Endung,Adj)}.
adjektiv(adj(Stamm), m, akk, s, _) 
	--> [Adj], {adjektiv_stamm(Stamm), Endung = en,concat(Stamm,Endung,Adj)}.
adjektiv(adj(Stamm), f, akk, s, _)
	--> [Adj], {adjektiv_stamm(Stamm), Endung = e, concat(Stamm,Endung,Adj)}.
adjektiv(adj(Stamm), n, akk, s, b)
	--> [Adj], {adjektiv_stamm(Stamm), Endung = e, concat(Stamm,Endung,Adj)}.
adjektiv(adj(Stamm), _, akk, p, b)
	--> [Adj], {adjektiv_stamm(Stamm), Endung = en,concat(Stamm,Endung,Adj)}.

adjektiv(adj(Stamm), m, nom, s, u)
	--> [Adj], {adjektiv_stamm(Stamm), Endung = er, concat(Stamm, Endung, Adj)}.
adjektiv(adj(Stamm), f, nom, s, u)
	--> [Adj], {adjektiv_stamm(Stamm), Endung = e, concat(Stamm, Endung, Adj)}.
adjektiv(adj(Stamm), n, nom, s, u)
	--> [Adj], {adjektiv_stamm(Stamm), Endung = es, concat(Stamm, Endung, Adj)}.
adjektiv(adj(Stamm), _, nom, p, u)
	--> [Adj], {adjektiv_stamm(Stamm), Endung = e, concat(Stamm, Endung, Adj)}.
adjektiv(adj(Stamm), n, akk, s, u)
	--> [Adj], {adjektiv_stamm(Stamm), Endung = es, concat(Stamm, Endung, Adj)}.
adjektiv(adj(Stamm), _, akk, p, u)
	--> [Adj], {adjektiv_stamm(Stamm), Endung = e, concat(Stamm, Endung, Adj)}.

% liefert Stamm aus Wörterbuch
adjektiv_stamm(A):- dict_adj(A,_,_,_).

% ==============================================================================================
% italienische Grammatik 

frase(s(NP, VP)) --> frase_nominale(NP, Numerus), frase_verbale(VP, Numerus).

% Nominalphrasen
frase_nominale(np(A, N), Numerus) 
	--> articolo(A, Genus, Numerus), nome(N, Genus, Numerus).

frase_nominale(np(N), Numerus) 
	--> nome(N, _, Numerus).

frase_nominale(np(Art,Adj,N), Numerus)
	--> articolo(Art, Genus, Numerus),
    aggettivo(Adj, Genus, Numerus, v),
    nome(N, Genus, Numerus).

frase_nominale(np(Art,Adj,N), Numerus)
	--> articolo(Art, Genus, Numerus),
    nome(N, Genus, Numerus),
    aggettivo(Adj, Genus, Numerus, h).
    
frase_nominale(np(Adj,N), p)
	--> aggettivo(Adj, Genus, p, v),
    nome(N, Genus, p).

frase_nominale(np(Adj,N), p)
	--> nome(N, Genus, p),
	aggettivo(Adj, Genus, p, h).

% Verbalphrasen
frase_verbale(vp(V), Numerus) --> verbo_intransitivo(V, Numerus).
frase_verbale(vp(V, NP), Numerus) --> verbo_transitivo(V, Numerus), frase_nominale(NP, _).

% Artikel
articolo(artb, m, s) --> [A], {A = il}.
articolo(artb, f, s) --> [A], {A = la}.
articolo(artb, m, p) --> [A], {A = i}.
articolo(artb, f, p) --> [A], {A = le}.
articolo(artu, m ,s) --> [A], {A = un}.
articolo(artu, f, s) --> [A], {A = una}.

% Verben
verbo_intransitivo(verb(V),Numerus) --> [V], {dict_verb(_,V,Numerus,i)}.
verbo_transitivo(verb(V),Numerus) --> [V], {dict_verb(_,V,Numerus,t)}.

% Nomen
nome(nomen(N), Genus, Numerus) --> [N], {dict_nomen(_,_,N,Genus,Numerus)}.

% Adjektive
% Baum(Stamm,Endung), Genus, Numerus, Stellung 
% Stellung v - vor, h - hinter
% Form 	r - ~e, ~a / ~o, ~i 
% 		u - ~e, ~i

% bestimmt Stamm aus Wörterbuch
aggettivo_base(Agg_base,Stellung,Form):- 
	dict_adj(_,Agg_base,Stellung,Form).    

% --- Stamm + Endung -> dekliniertes aggetivo ---
% maskulin Singular
aggettivo(adj(Stamm), m, s, Stellung) 
	--> [Adj], {aggettivo_base(Stamm,Stellung,r), Endung = o, concat(Stamm,Endung,Adj)};
    	[Adj], {aggettivo_base(Stamm,Stellung,u), Endung = e, concat(Stamm,Endung,Adj)}.

% feminim Singular
aggettivo(adj(Stamm), f, s, Stellung)
	--> [Adj], {aggettivo_base(Stamm,Stellung,r), Endung = a, concat(Stamm,Endung,Adj)};
    	[Adj], {aggettivo_base(Stamm,Stellung,u), Endung = e, concat(Stamm,Endung,Adj)}.

% maskulin Plural
aggettivo(adj(Stamm), m, p, Stellung)
	--> [Adj], {aggettivo_base(Stamm,Stellung,r), Endung = i, concat(Stamm,Endung,Adj)};
    	[Adj], {aggettivo_base(Stamm,Stellung,u), Endung = i, concat(Stamm,Endung,Adj)}.

% feminim Plural
aggettivo(adj(Stamm), f, p, Stellung)
	--> [Adj], {aggettivo_base(Stamm,Stellung,r), Endung = e, concat(Stamm,Endung,Adj)};
    	[Adj], {aggettivo_base(Stamm,Stellung,u), Endung = i, concat(Stamm,Endung,Adj)}.

% ==============================================================================================
% Wörterbuch

% Adjektive (enthält nur die deutsche und italienischen Grundformen, 
% die Endungen werden durch Grammatikregeln bestimmt und angehängt)
% Parameter: (Deutsch, Italienisch, ital. Stellung, ital. Deklinationsform)
dict_adj(AD,AI,Stellung,Form):-
    member((AD,AI,Stellung,Form),
    [(rot,ross,h,r),(jung,giovan,v,u),(blond,biond,h,r),
      (klein,piccol,v,r),(schnell,veloc,h,u),(neu,nuov,v,r)]).

% Nomen (Deutsch, Genus im Deutschen , Italienisch, Genus im Italienischen, Numerus)
dict_nomen(ND, GD, NI, GI, Numerus) :- 
    member((ND, GD, NI, GI, Numerus), 

% Plural
[(katzen,f,gatti,m,p), (mäuse,f,topi,m,p), (väter,m,padri,m,p), 
 (häuser,n,case,f,p), (mädchen,n,ragazze,f,p), (autos,n,macchine,f,p),
 (frauen,f,donne,f,p),
% Singular
 (paul,m,paolo,m,s),(katze,f,gatto,m,s),(maus,f, topo,m,s),(vater,m,padre,m,s),
 (haus,n,casa,f,s),(mädchen,n,ragazza,f,s),(auto,n,macchina,f,s),
 (frau,f,donna,f,s)]).

% Verben (Deutsch,Italienisch,Numerus,Transitivität)
dict_verb(VD,VI,Numerus,Trans):- 
    member((VD,VI,Numerus,Trans), 
	[(jagt,caccia,s,t),(sieht,vede,s,t),(fährt,guida,s,t),(liebt,ama,s,t),(ruft,chiama,s,t),
	 (jagen,cacciano,p,t),(sehen,vedono,p,t),(fahren,guidano,p,t),(lieben,amano,p,t),(rufen,chiamano,p,t),
	 (schläft,dorme,s,i),(rennt,corre,s,i),(jagt,caccia,s,i),(ruft,chiama,s,i),
	 (schlafen,dormono,p,i),(rennen,coronno,p,i),(jagen,cacciano,p,i),(rufen,chiamano,p,i)]).

dict_name(nomen(D), nomen(I)) 
	--> {member((D, I), [(paul, paolo)])}.

% ==============================================================================================
% Regeln zum Übersetzen für einzelne Satzstrukturen

trans(np(nomen(ND,Numerus)), np(nomen(NI))):- 
    dict_nomen(ND,_,NI,_,Numerus).
trans(np(Art, nomen(ND,Numerus)), np(Art, nomen(NI))) :- 
    dict_nomen(ND,_,NI,_,Numerus).
trans(np(Art, adj(AD), nomen(ND,Numerus)), np(Art, adj(AI), nomen(NI))):-
    dict_adj(AD,AI,_,_), dict_nomen(ND,_,NI,_,Numerus).
trans(np(adj(AD), nomen(ND,Numerus)), np(adj(AI), nomen(NI))):-
    dict_adj(AD,AI,_,_), dict_nomen(ND,_,NI,_,Numerus).


trans(vp(VD), vp(VI)) :- trans(VD, VI).
trans(vp(VD, NPD), vp(VI, NPI)) :- trans(VD, VI), trans(NPD, NPI).
trans(verb(VD), verb(VI)) :- dict_verb(VD,VI,_,_).
trans(adj(AD), adj(AI)):- dict_adj(AD,AI,_,_).

% ==============================================================================================
% Steuerung der gesamten Übersetzung

trans(D, I) :-
	phrase(satz(s(NPD, VPD)), D),
	trans(NPD, NPI),
	trans(VPD, VPI),
	phrase(frase(s(NPI, VPI)), I).
