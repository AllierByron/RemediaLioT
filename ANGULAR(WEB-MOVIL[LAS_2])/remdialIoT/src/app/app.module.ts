import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { HttpClientModule, HTTP_INTERCEPTORS } from '@angular/common/http';


import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { SensorComponent } from './sensor/sensor.component';

//servicios
import { SensorService } from './services/sensor/sensor.service';

@NgModule({
  declarations: [
    AppComponent,
    SensorComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule
  ],
  providers: [
    SensorService,
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
