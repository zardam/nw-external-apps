sapin1(h):={
si (h<5) alors
avance(h);
recule(h);
sinon
tourne_droite(60);
sapin1(h/2);
tourne_gauche(120);
sapin1(h/2):
tourne_droite(60);
avance(h/4);
sapin1(3*h/4);
recule(h/4);
fsi;
};
sapin2(h,n):={
si (n==0) alors
avance(h);
recule(h);
sinon
tourne_droite(60);
sapin2(h/2,n-1);
tourne_gauche(120);
sapin2(h/2,n-1):
tourne_droite(60);
avance(h/4);
sapin2(3*h/4,n-1);
recule(h/4);
fsi;
};