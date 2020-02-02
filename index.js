"use strict";

/*
Copyright (C) 2019 Damien Nicolet

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

angular.module('nwas', ['ngSanitize', 'pascalprecht.translate']).controller('main', function($scope, $http, apps, $translate) {

  $scope.locale = $translate.use();
  $scope.apps = apps;
  $scope.selectedApps = [];
  $scope.customFiles = [];
  $scope.webUSB = typeof navigator.usb !== 'undefined';

  $scope.reload = function reload() {
    document.location.reload();
  }

  $scope.addApplication = function addApplication(app) {
    if($scope.selectedApps.indexOf(app) < 0) {
      $scope.selectedApps.push(app);
    }
  };

  $scope.removeApplication = function removeApplication(app) {
    let index = $scope.selectedApps.indexOf(app);
    if(index >= 0) {
      $scope.selectedApps.splice(index, 1);
    }
  };

  $scope.removeFile = function removeFile(file) {
    let index = $scope.customFiles.indexOf(file);
    if(index >= 0) {
      $scope.customFiles.splice(index, 1);
    }
  };

  let link = function link(app, linkerScript) {
    let output;
    return new Promise(function (resolve, reject) {
      let linkModule = {
        arguments: ["/input.elf", "-o", "/output.elf", "-T", "/linker.ld"],
        preRun: [ () => {
          linkModule.FS.writeFile("/linker.ld", linkerScript);
          linkModule.FS.createPreloadedFile("/", "input.elf", "apps/" + app.name + "/app.elf", true, true);
        }],
        postRun: [ () => {
          output = linkModule.FS.readFile("/output.elf");
        }]
      };
      ld(linkModule).then( () => {
        let objcopyModule = {
          arguments: ["-O", "binary", "/input.elf", "/output.bin"],
          preRun: [ () => {
              objcopyModule.FS.writeFile("/input.elf", output);
          }],
          postRun: [ () => {
              resolve(objcopyModule.FS.readFile("/output.bin"));
          }]
        };
        objcopy(objcopyModule);
      });
    });
  };

  let loadLinkerScript = function loadLinkerScript() {
    return new Promise(function(resolve, reject) {
      $http.get("apps/external.ld", {responseType: "text"})
        .then(function successCallback(response) {
          resolve(response.data);
        }, function errorCallback(response) {
          reject("Unable to download linker script");
        });
    });
  }

  let buildArchive = async function buildArchive(applications, files) {
    if(applications.length == 0 && files.length == 0) {
      return new Promise(function(resolve, reject) {
        resolve(new Uint8Array(0x200));
      });
    } else {
      let linkerScript = await loadLinkerScript();
      let address = 0;
      let tar = new tarball.TarWriter();

      for(let i = 0; i < applications.length; i++) {
        let app = applications[i];
        console.log("Processing", app.name, address);
        $scope.$apply(function() {
          $scope.lastAction = $translate.instant("PROCESSING") + " " + app.name;
        });
        let binary = await link(app, linkerScript.replace("(0)", "(" + address + ")"));
        address += 0x200 * Math.floor((binary.length + 1023) / 0x200);
        console.log("Taring", app.name)
        tar.addFileArrayBuffer(app.name, binary, {mode: "775"});
      }

      for(let i = 0; i < files.length; i++) {
        let file = files[i];
        console.log("Taring", file.name)
        $scope.$apply(function() {
          $scope.lastAction = $translate.instant("ADDING") + " " + file.name;
        });
        tar.addFileArrayBuffer(file.name, file.binary, {mode: "664"});
      }

      console.log("Build archive done");
      return tar.write();
    }
  }

  let uploadFile = async function uploadFile(selectedDevice, dfuDescriptor, file, manifest) {
    console.log("uploading", dfuDescriptor);
    let interfaces = dfu.findDeviceDfuInterfaces(selectedDevice);
    console.log("interfaces", interfaces);
    if (interfaces.length == 0) {
      throw "The selected device does not have any USB DFU interfaces.";
    }
    interfaces[0].name = dfuDescriptor;
    let device = new dfuse.Device(selectedDevice, interfaces[0]);
    console.log("device", device);
    await device.open();
    try {
      let status = await device.getStatus();
      if (status.state == dfu.dfuERROR) {
        await device.clearStatus();
      }
    } catch (error) {
      console.log("Failed to clear status");
    }
    device.logProgress = function(done, total) {
      console.log("progress", done, total);
      $scope.$apply(function() {
        $scope.progress = (done / total) * 100;
      })
    }
    device.logInfo = function(message) {
      console.log(message);
      $scope.$apply(function() {
        if(message.startsWith("Erasing")) {
          $scope.lastAction = $translate.instant("DFU_ERASING");
        } else if (message.startsWith("Copying")) {
          $scope.lastAction = $translate.instant("DFU_COPYING");
        } else if (message.startsWith("Wrote")) {
          $scope.lastAction = $translate.instant("DFU_WROTE");
        } else {
          $scope.lastAction = message;
        }
      })
    }
    await device.do_download(2048, file, manifest).then(
      () => {
        console.log("done", dfuDescriptor);
      },
      error => {
        throw error;
      }
    );
  }

  $scope.upload = function upload() {
    delete $scope.error;
    navigator.usb.requestDevice({
      filters: [{
        vendorId: 0x0483,
        productId: 0xa291
      }]
    }).then(
      async selectedDevice => {
        console.log("Selected device", selectedDevice);
        $scope.$apply(function() {
          $scope.uploading = true;
        });
        let archive = await buildArchive($scope.selectedApps, $scope.customFiles);
        console.log("Archive", archive);
        await uploadFile(selectedDevice, "@External Flash /0x90200000/32*064Kg,64*064Kg", archive, false);
        $scope.$apply(function() {
          $scope.allDone = true;
        });
      }
    ).catch(error => {
      $scope.$apply(function() {
        $scope.error = error;
        $scope.allDone = false;
      });
    });
  };

  $scope.getFile = function getFile(el) {
    let file = el[0].files[0];
    let reader = new FileReader();

    reader.addEventListener("load", function() {
      $scope.$apply(function() {
        let found = $scope.customFiles.find(e => e.name == file.name);
        if(found) {
          console.log("replaced", file);
          found.binary = reader.result;
        } else {
          console.log("loaded", file);
          $scope.customFiles.push(
            {name: file.name, binary: reader.result}
          );
        }
        $(file).val("");
      })
    }, false);

    $scope.$apply(function() {
      if (file) {
        console.log("loading", file);
        reader.readAsArrayBuffer(file);
      }
    });
  };

  let loadFirmwareFile = function loadFirmwareFile(file) {
    return new Promise(function (resolve, reject) {
      $http.get(file, {responseType: "arraybuffer"})
      .then(function(response) {
        resolve(response.data);
      }, function(error) {
        console.log(error);
        reject("Unable to load firmware file");
      });
    });
  }

  $scope.uploadFirmware = function uploadFirmware(version) {
    delete $scope.error;
    navigator.usb.requestDevice({
      filters: [{
        vendorId: 0x0483,
        productId: 0xa291
      }]
    }).then(
      async selectedDevice => {
        console.log("Selected device", selectedDevice);
        $scope.$apply(function() {
          $scope.uploading = true;
        });
        let internal = await loadFirmwareFile("firmware/" + version + ".internal.bin");
        let external = await loadFirmwareFile("firmware/" + version + ".external.bin");
        await uploadFile(selectedDevice, "@External Flash /0x90000000/08*004Kg,01*032Kg,31*064Kg", external, false);
        await uploadFile(selectedDevice, "@Internal Flash /0x08000000/04*016Kg", internal, true);
        $scope.$apply(function() {
          $scope.allDone = true;
        });
      }
    ).catch(error => {
      $scope.$apply(function() {
        $scope.error = error;
        $scope.allDone = false;
      });
    });
  };

}).directive("ngFileSelect", function() {
  return {
    link: function($scope, el) {
      el.bind("change", function(e) {
        $scope.getFile(el);
      })
    }
  }
}).config(function ($translateProvider) {
  $translateProvider
    .translations('en', {
      TITLE: 'Unofficial N110 application repository',
      LEAD: 'Here you will find some installable applications for a N110 calculator.',
      FIRMWARE: 'To install a compatible firmware on your calculator, please do an update on the official site, then go ',
      DISCLAIM: 'For more information (or filling an issue) please go ',
      HERE: 'here',
      NO_WEB_USB: 'Your browser does not support WebUSB, please use',
      SELECTED_APPLICATIONS: 'Selected applications',
      REMOVE: 'Remove',
      CUSTOM_FILE: 'Custom file',
      INSTALL: 'Install',
      AVAILABLE_APPLICATIONS: 'Available applications',
      ADD: 'Add',
      ACKNOWLEDGMENTS: 'Acknowledgments',
      ALL_DONE: 'All done, click here to reload the page.',
      ERROR: 'An error occurred',
      PLEASE_RELOAD: 'Please reload the page',
      PROCESSING: "Processing",
      ADDING: "Adding",
      DFU_ERASING: "Erasing",
      DFU_COPYING: "Copying data",
      DFU_WROTE: "Done",
      OR: "or",
    })
    .translations('fr', {
      TITLE: 'Dépôt d\'application N110 non officiel',
      LEAD: 'Vous trouverez ici quelques application installables sur une calculatrice N110.',
      FIRMWARE: 'Pour installer un micrologiciel compatible, veuillez mettre à jour votre calculatrice sur le site officiel, puis vous rendre ',
      DISCLAIM: 'Pour plus d\'informations (ou soumettre un problème) veuillez vous rendre ',
      HERE: 'ici',
      NO_WEB_USB: 'Votre navigateur ne supporte pas WebUSB, veuillez utiliser',
      SELECTED_APPLICATIONS: 'Applications sélectionnées',
      REMOVE: 'Supprimer',
      CUSTOM_FILE: 'Fichier local',
      INSTALL: 'Installer',
      AVAILABLE_APPLICATIONS: 'Applications disponibles',
      ADD: 'Ajouter',
      ACKNOWLEDGMENTS: 'Remerciements',
      ALL_DONE: 'Terminé, cliquez ici pour recharger la page.',
      ERROR: 'Une erreur est survenue',
      PLEASE_RELOAD: 'Veuillez recharger la page',
      PROCESSING: "Traitement de",
      ADDING: "Ajout de",
      DFU_ERASING: "Effacement",
      DFU_COPYING: "Copie des fichiers",
      DFU_WROTE: "Terminé",
      OR: "ou",
    })
    .registerAvailableLanguageKeys(['en', 'fr'], {
      'en_*': 'en',
      'fr_*': 'fr',
      '*': 'en',
    })
    .determinePreferredLanguage()
    .useSanitizeValueStrategy('sanitizeParameters');
});

