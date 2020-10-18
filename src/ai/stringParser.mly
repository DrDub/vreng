
/*s Analyse syntaxique et construction de l'arbre */
%{
  open RequeteSyntax

  let cur_loc () = symbol_start(), symbol_end()

  let parse_error _ = raise Parsing.Parse_error
%}

/*s D�claration des tokens. */

%token EOF 
%token <float> VFLOAT

/*s Point d'entr�e de la grammaire. */

%type <RequeteSyntax.objet> stringdepart
%start stringdepart

%%

/*s R�gles de grammaire. */

stringdepart :
| VFLOAT VFLOAT VFLOAT VFLOAT VFLOAT VFLOAT VFLOAT {ObjetTrouve(Coord($1,$2,$3,$4),Dimension($5,$6,$7))}
| EOF {ObjetTrouve(NA,Dimension(0.0,0.0,0.0))}
