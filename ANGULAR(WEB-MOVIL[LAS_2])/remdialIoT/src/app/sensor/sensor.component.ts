import { Component } from '@angular/core';
import { SensorService } from '../services/sensor/sensor.service';

@Component({
  selector: 'app-sensor',
  templateUrl: './sensor.component.html',
  styleUrls: ['./sensor.component.css']
})
export class SensorComponent {
  protected activado = false;

  constructor(private servicioSensor: SensorService){ }

  activarSensor(){
    this.servicioSensor.activarSensor().subscribe({
      next:res=>{
        setTimeout(() => {
          this.getData();
        }, 2000);
        console.log(res);
      },
      error:err=>{
        console.log(err);
        
      }
    });
  }

  desactivarSensor(){
    this.servicioSensor.desactivarSensor().subscribe({
      next:res=>{
        console.log(res);
      },
      error:err=>{
        console.log(err);
        
      }
    });
  }

  getData(){
    this.servicioSensor.getData().subscribe({
      next:res=>{
        if(res.data.msj == "true"){
          this.activado = true;
        }
        console.log(res);
      },
      error:err=>{
        console.log(err);
        
      }
    });
  }

}
