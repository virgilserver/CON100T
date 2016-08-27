var express = require('express');
var router = express.Router();

var firebase = require('firebase');

firebase.initializeApp({
  databaseURL: "https://con100t-f2921.firebaseio.com/"
});

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Express' });
});

router.get('/enchufes', function(req, res, next) {
    res.writeHead(200,{'Content-Type': 'application/json'});
    var db = firebase.database();
    var ref = db.ref("/arduino1/enchufes");
    ref.once("value",function (snapshot) {
        console.log(snapshot.val());

        res.end(JSON.stringify(snapshot.val()));
    });

});

router.get('/push', function(req, res, next) {
    var horaFormat = req.query.ft;
    var horaTS = horaFormat.replace(/:/g,'');
    var option = req.query.ca;
    var corrienteA = req.query.cA;
    var corrienteB = req.query.cB;
    var corrienteC = req.query.cC;
    var corrienteD = req.query.cD;
    var voltajeTotal = req.query.vt;
    var fechaFormat = req.query.fF;
    var fechaTS = fechaFormat.replace(/-/g,'')
    var corrienteTotal = req.query.cT;


    var db = firebase.database();
    var ref = db.ref("arduino1/");
    var registroRef = ref.child("registros");

    var newRegistroRef = registroRef.push();
    newRegistroRef.set({
        corrientea: corrienteA,
        corrienteb: corrienteB,
        corrientec: corrienteC,
        corriented: corrienteD,
        potenciaa: corrienteA * voltajeTotal,
        potenciab: corrienteB * voltajeTotal,
        potenciac: corrienteC * voltajeTotal,
        potenciad: corrienteD * voltajeTotal,
        voltajet: voltajeTotal,
        horaTS: horaTS,
        horaFormat: horaFormat,
        fechaTS: fechaTS,
        fechaFormat: fechaFormat,
        corrienteT: corrienteTotal

    });


    console.log("voltaje total = : " + voltajeTotal );
    console.log("corriente a = " + corrienteA);
    console.log("corriente b = " + corrienteB);
    console.log("corriente c = " + corrienteC);
    console.log("corriente d = " + corrienteD);
    console.log("fechaFormat = " + fechaFormat);
    console.log("corriente Total= " + corrienteTotal);
    console.log("hora = " + horaFormat);
    console.log("pruebilla = " + option);
    res.end();
});


module.exports = router;
