import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

@Injectable({
  providedIn: 'root'
})
export class SensorService {
  private url = "http://localhost:3000/sensor";
  
  constructor(private http:HttpClient) { }

  activarSensor(){
    return this.http.get<any>(this.url+"/activar");
  }

  desactivarSensor(){
    return this.http.get<any>(this.url+"/desactivar");
  }

  getData(){
    return this.http.get<any>(this.url+"/getData");
  }

}
