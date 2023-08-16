const amongoose = require("mongoose");

const medicionSchema = new amongoose.Schema({
    noRegistro: Number,
    temperatura: String,
    humedad: String
    
});

amongoose.model("medicion", medicionSchema);