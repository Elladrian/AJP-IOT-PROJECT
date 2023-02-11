import { Component} from '@angular/core';

import { HttpClient, HttpHeaders } from '@angular/common/http';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  time: number = 0;
  timeText: string = '0s';
  
  deviceAddress: string = "192.168.1.19";
 
  constructor(private http: HttpClient){}

 removeTime() {
  console.log("time decrement");
  this.time -= 15;
  if (this.time < 0) {
    this.time = 0;
  }
  console.log(this.time);
  this.timeUpdate();
  }

  resetTimer() {
    console.log("time reset");
    this.time = 0;
    console.log(this.time);
    this.timeUpdate();
  }

  addTime() {
    console.log("time increment");
    this.time += 15;
    console.log(this.time);
    this.timeUpdate();
  }

  timeUpdate() {
    this.timeText = this.time + "s";
  }

  post() {
    if(this.time <= 0 ){
      this.timeText = `${this.time}s cant be sent`;
    }
    this.timeText = `Sended ${this.time}s`;

    let headers = new HttpHeaders()
      .set('content-type', 'application/x-www-form-urlencoded')
      .set('Access-Control-Allow-Origin', '*')

    let body = `time=${this.time}`;

    let resp = this.http.post<string>("/api/set-timer", body, {headers: headers})
      .subscribe(resp => {
      console.log(resp);
    })

  }
}
