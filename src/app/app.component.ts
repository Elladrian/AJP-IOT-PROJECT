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
  
  deviceAddress: string = "127.0.0.0";
 
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
    this.timeText = "Posted";
    this.time = 0;

    let headers = new HttpHeaders()
      .set('content-type', 'application/json')
      .set('Access-Control-Allow-Origin', '*');

    let options = {headers: headers}

    let resp = this.http.post<string>("http://" + this.deviceAddress + "/post", {time: this.time} ,options)
      .subscribe(resp => {
      console.log(resp);
    })

  }
}
