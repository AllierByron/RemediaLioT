const amongoose = require("mongoose");

const medicionSchema = new amongoose.Schema({
    datos_sensor: [{
        noRegistro: Number,
        temperatura: String,
        humedad: String
    }]
}, {timestamps: true});

amongoose.model("medicion", medicionSchema);