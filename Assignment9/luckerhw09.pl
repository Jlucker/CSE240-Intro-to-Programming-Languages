%00000000000000000000000000000000000000000000000000000000
% Programmed by:  Justin Lucker
% Date: 04/25/2014
% Email: jlucker@asu.edu
% Assignment: Homework #9
%00000000000000000000000000000000000000000000000000000000

%facts
phylum(mammalia, chordata).
phylum(aves, chordata).

class(carnivora, mammalia).
class(sphenisciformes, aves).
%------------->ADDED FACT<-------------
class(struthionformes, aves).

order(canidae, carnivora).
order(felidae, carnivora).
order(spheniscidae, sphenisciformes).
%------------->ADDED FACT<-------------
order(dromaiidae, struthionformes).

family(canis, canidae).
family(acinonyx, felidae).
family(panthera, felidae).
family(puma, felidae).
family(pygoscelis, spheniscidae).
%------------->ADDED FACT<-------------
family(dromaius, dromaiidae ).
family(leopardus, felidae).

%http://en.wikipedia.org/wiki/Cheetah
genus(a_jubatus, acinonyx).
%http://en.wikipedia.org/wiki/Puma
%uneeded, see mountain lion.
%http://en.wikipedia.org/wiki/Jaguar
genus(p_onca, panthera).
%http://en.wikipedia.org/wiki/Panther
%unneeded, see leopard.
%http://en.wikipedia.org/wiki/Tiger
genus(p_tigris, panthera).
%http://en.wikipedia.org/wiki/Leopard
genus(p_pardus, panthera).
%http://en.wikipedia.org/wiki/Snow_leopard
genus(p_uncia, panthera).
%http://en.wikipedia.org/wiki/Lion
genus(p_leo, panthera).
%http://en.wikipedia.org/wiki/Mountain_lion
genus(p_concolor, puma).
%http://en.wikipedia.org/wiki/Gray_Wolf
genus(c_lupus, canis).
%http://en.wikipedia.org/wiki/Ad%C3%A9lie_Penguin
genus(p_adeliae, pygoscelis).
%------------->ADDED FACT<-------------
%http://en.wikipedia.org/wiki/Ocelot
genus(l_pardalis, leopardus).
%http://en.wikipedia.org/wiki/Emu
genus(d_novaehollandiae, dromaius).

% Add facts (phylum, class, order, family, genus - as needed) that
% defines a new (real) species in chordata. You must add at least two
% facts to the factbase. [5pts]
%-------->TOTAL ADDED FACTS: 6<--------

% Define a rule that determines species. For example: species(pygoscelis)
% returns no, species(X) returns X=p_adeliae, X=c_lupus, etc. [5pts]
%----->Note:  Press ; to show multiple results<-----
species(X) :- genus(X, _).

% Define a rule that determines if a species is in a family. For
% example: in_family(p_adeliae, felidae) returns no,
% in_family(p_adeliae, X) returns X=spheniscidae. [5pts]

in_family(X, Y) :- genus(X, X1), family(X1, Y).

% Define a rule that determines if a species is in an order. For
% example: in_order(Species, Order). [3pts]

in_order(X, Y) :- in_family(X, X1), order(X1, Y).

% Define a rule that determines if a species is in a class. For
% example: in_class(Species, Class). [3pts]

in_class(X, Y) :- in_order(X, X1), class(X1, Y).

% Define a rule that determines if a species is in a phylum. For
% example: in_phylum(Species, Phylum). [3pts]

in_phylum(X, Y) :- in_class(X, X1), phylum(X1, Y).

% Define a rule that determines if two species are in the same genus.
% For example: same_genus(p_adeliae, p_tigris) returns no,
% same_genus(p_tigris, X) returns X=p_onca, X=p_pardus, etc. [5pts]
%----->Note:  Press ; to show multiple results<-----

same_genus(X, Y) :- genus(X, X1), genus(Y, X1); genus(X, Y1), genus(Y1, _).

% Define a rule that determines if two species are in the same family.
% For example: same_family(Species1, Species2). [3pts]

same_family(X, Y) :- in_family(X, X1), in_family(Y, X1).  

% Define a rule that determines if two species are in the same order.
% For example: same_order(Species1, Species2). [3pts]

same_order(X, Y) :- in_order(X, X1), in_order(Y, X1).