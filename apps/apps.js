'use strict';

angular.module('nwas').service('apps', function() {
  return [
    { name: "KhiCAS", description: {en: "Computer algebra system", fr: "Système de calcul formel"}, link: "https://www-fourier.ujf-grenoble.fr/~parisse/" },
    { name: "Periodic", description: {en: "Periodic table of elements", fr: "Tableau périodique des éléments"}, link: "https://bitbucket.org/m4x1m3/nw-atom" },
    { name: "Nofrendo", description: {en: "NES emulator", fr: "Émulateur NES"}, link: "https://github.com/zardam/numworks-nofrendo" },
  ];
});