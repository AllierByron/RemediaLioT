var express = require('express');
var router = express.Router();
const mongoose = require("mongoose");
const medicion = mongoose.model("medicion");

//variables mias
var activo = false;
let activar = false;
var llamadas = 0;
let data ={
    temperatura:"",
    humedad:"",
    msj:"",
}

/* GET users listing. */
router.post('/', function(req, res) {
    llamadas++;
    activo = true;
    res.status(201).send('temperatura:'+req.body.temperatura+"\n"+    
                          'humedad: '+req.body.humedad+"\n"+    
                          'msj: '+req.body.msj);
    data.temperatura = req.body.temperatura;
    data.humedad = req.body.humedad;
    data.msj = req.body.msj;
    console.log('temperatura:'+req.body.temperatura+"\n"+    
                'humedad: '+req.body.humedad+"\n"+    
                'msj: '+req.body.msj);
    registrarMedicion(data);

    checkChanges();
});

//function to check if the sensor its still transmiting data
function checkChanges(){
    let oldValue = llamadas;
    // console.log('old:'+llamadas);
    
    setTimeout(() => {
        // console.log('new:'+llamadas);    
        let newValue = llamadas;
        if(oldValue == newValue){
            activo = false;
            // console.log('igual?: '+(oldValue == newValue));
        }    
    }, 8000);    
}

router.get('/getData', async(req, res)=>{
    if(activo){
        res.status(200).send({data});
    }else{
        res.status(204).send({data:{msj: "No esta conectado"}});
    }
});

router.get('/activar', (req, res)=>{
    activar = true;
    res.status(200).send({msj:activar});
});

router.get('/desactivar',(req, res)=>{
    activar = false;
    res.status(200).send({msj:"desact"});
});

router.get('/', async(req, res)=>{
    if(activar){
        res.status(200).send(true);
    }else{
        res.status(200).send(false);
    }
});

router.put('/', async(req, res)=>{
    let data = await usuario.findOne({
        values:[
            {
                data:{
                    temperatura: 49
                }
            }
        ]
    });

    let newData = data.values[0].data.temperatura = req.body.temperaturax;

    await newData.save();

    res.status(200).send({newData});
});

router.delete('/', async(req, res)=>{
    let data = await usuario.findOne({
        values:[
            {
                data:{
                    temperatura: 49
                }
            }
        ]
    });

    //lo dejamos vacio para elimiarlo logicamente

    let newData = data.values[0].data.temperatura =" ";

    await newData.save();

    res.status(200).send({newData});
});

router.patch('/', async(req, res)=>{
    var data = await usuario.findOne({values:[{data:{humedad: 49}}]});

    var datosNuevos = data.values[0].data.humedad = req.body.humedad;

    await datosNuevos.save();

    res.status(204).send({datosNuevos});
});


async function registrarMedicion(data){
    
    let cant = await medicion.findOne({}).sort({_id:-1}).limit(1);
    console.log(cant);
    if(!cant) {
        cant={noRegistro: 0};
    }else{
        let elArray = await medicion.aggregate([{
            $project:
            { item: 1, ultimo: { $size: "$datos_sensor" } }
        }]);
        if(elArray) cant.noRegistro = elArray[0].ultimo;
    }
    console.log("Cant:");
    console.log(cant);

    // console.log("Ultimo indice en el array: " + num);
        console.log("Numero de reg: " + cant.noRegistro);

    if (cant.noRegistro>=100) cant.noRegistro = 0;
        console.log("Numero de reg: " + cant.noRegistro);

    if(cant.noRegistro==0){
        console.log("nuevo");
        let medi = await new medicion({
            datos_sensor: [{
                noRegistro: parseInt(cant.noRegistro)+1,
                temperatura: data.temperatura+"C°",
                humedad: data.humedad+"%"
            }]
        });
        await medi.save();
        // console.log(medi);
        return {medi};
    }

        console.log("updating");
        let medi = await medicion.findOneAndUpdate(
            { _id: cant._id },
            {
                $push: {
                    datos_sensor: {
                        noRegistro: (cant.noRegistro)+1,
                        temperatura: data.temperatura+"C°",
                        humedad: data.humedad+"%"
                    }
                }
            },
            { new: true }
        );

    return {medi};
}

module.exports = router;