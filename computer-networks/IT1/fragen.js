"use strict";

//Objekte: Model(m), View(v), Presenter(p), a weiß ob Mathe oder allgemein gewählt ist
let m, v, p, a;

//EventListener: DOMLoaded, starte erst nach vollständigem Laden
document.addEventListener('DOMContentLoaded', function(){
	if(document.getElementById("module").innerHTML == "Mathematik lernen"){
		a = "m";
	}else if(document.getElementById("module").innerHTML == "Allgemeinbildung"){
		a = "a";
	}
	m = new Model();
	p = new Presenter();
	v = new View(p);
	p.setModelAndView(m,v);
});

//Model
class Model{
	// Fragen als JSON
	question = {
		"mathe": [
			{"a": "x^2+x^2 =", "l": ["2x^2", "x^4", "x^8", "2x^4"]},
			{"a": "x^2*x^2 =", "l": ["x^4", "x^2", "2x^2", "4x"]},
			{"a": "pq\\textrm{-Formel: } x_{1,2} =", "l": ["-\\frac{p}{2} \\pm \\sqrt{\\frac{p}{2}^2 -q}", "p+q", "p*q", "p:q"]},
			{"a": "(a+b)^2 =", "l": ["a^2+2ab+b^2", "2xy", "a^2+b^2", "a*b*(ab)^2"]},
			{"a": "(a+b)*(a-b) =", "l": ["a^2-b^2", "a-b", "a*b", "a^3"]},
			{"a": "\\int_{0}^{1} 2x \\textrm{ d}x =", "l": ["1.0", "100", "0", "x^2"]},
			{"a": "f(x) = \\textrm{a}^{2}x^{3} + 0.5x^{2} \\textrm{ ableiten}", "l": ["3\\textrm{a}^{2}x^{2}+x", "2\\textrm{a}", "3x^{2}+x", "4y"]},
			{"a": "5 \\textrm{ mod } 4 = ", "l": ["1", "2", "3", "0"]},
			{"a": "25 \\% \\textrm{ von } 95 \textrm{?}", "l": ["23.75", "27.53", "75.23", "32.75"]},
			{"a": "\\textrm{Die EULERsche Identität?}", "l": ["\\textrm{e}^{\\textrm{i}\\pi} + 1 = 0", "1", "0", "\\textrm{e}"]},
		],
		"allgemein": [
			{"a": "Geburtsjahr Karls des Großen?", "l": ["747", "828", "650", "1150"]},
			{"a": "Todestag Gaius Iulius Caesars?", "l": ["15. März", "1. April", "3. Juni", "13. Juli"]},
			{"a": "Letzer Sächsischer König?","l": ["Friedrich August III", "Friedrich August II", "August II", "Friedrich IV"]},
			{"a": "Gründung der BRD?", "l": ["23. Mai 1949", "24.Mai 1949", "3. Oktober 1948", "1. März 1951"]},
			{"a": "Formelzeichen für Leistung? (Physik)", "l": ["P", "p", "L", "Q"]},
			{"a": "Elementsymbol für Selen?", "l": ["Se", "S", "Sl", "Ca"]},
			{"a": "Französisch: Guten Tag?", "l": ["bon jour", "salut", "hello", "ciao"]},
			{"a": "Was ist keine Zutat für Spaghetti Carbonara?", "l": ["Sahne", "Schweinebacke", "Käse", "Pfeffer"]},
			{"a": "Wer schrieb Buddenbrooks - Verfall einer Familie?","l": ["Thomas Mann", "Heinrich Mann", "J. W. von Goethe", "J. K. Rowling"]},
			{"a": "Additive Farbmischung von Rot und Blau ergibt?", "l": ["Magenta", "Cyan", "Schwarz", "Violett"]}
		]
	};

	constructor(){
		console.log("MODEL initialisiert");
	}

	getTasks(index){
		if(a == "m"){
			console.log("MODEL: neue Frage (Mathe): " + this.question.mathe[index].a);
			return this.question.mathe[index].a;
		}else if(a == "a"){
			console.log("MODEL: neue Frage (allgemein): " + this.question.allgemein[index].a);
			return this.question.allgemein[index].a;
		}

	}

	getAnswers(index){
		if(a == "m") {
			console.log("MODEL: neue Antworten (Mathe) " + this.question.mathe[index].l);
			return this.question.mathe[index].l;
		}else if(a == "a"){
			console.log("MODEL: neue Antworten (allgemein) " +this.question.allgemein[index].l);
			return this.question.allgemein[index].l;
		}
	}
}

//View
class View{

	n;

	constructor(p){
		this.p = p;
		this.setHandler();
		console.log("VIEW: initialisiert");
		document.getElementById("A").setAttribute("name", "0");
		document.getElementById("B").setAttribute("name", "1");
		document.getElementById("C").setAttribute("name", "2");
		document.getElementById("D").setAttribute("name", "3");
	}

	//statische Eventhandler setzen
	//this soll bei Eventauflösung auf View zeigen, nicht auf Event --> bind(this)
	setHandler(){
		//answer Container mit Button
		document.getElementById("answer").addEventListener("click", this.check.bind(this));
		//Startknopf --> Model + Progressbar zurücksetzen
		document.getElementById("start").addEventListener("click", this.startQuiz.bind(this));
		//Nächste Frage laden
		document.getElementById("next").addEventListener("click", this.nextQuestion.bind(this));
	}

	check(event){
		console.log(event.type + " " + event.target.nodeName);

		let num;
		if(event.target.nodeName.toLowerCase() === "button"){
			num = Number(event.target.attributes.getNamedItem("name").value);

			if(this.p.evaluate(num)){
				//richtige Antwort gewählt --> entsprechender Button grün + Progressbar
				this.n++;
				let p = document.getElementById("pbar").value;
				p += 1;
				document.getElementById("pbar").value = p;
				switch (num){
					case 0:{
						document.getElementById("A").style.backgroundColor = "green";
						break;
					}
					case 1:{
						document.getElementById("B").style.backgroundColor = "green";
						break;
					}
					case 2:{
						document.getElementById("C").style.backgroundColor = "green";
						break;
					}
					default:{
						document.getElementById("D").style.backgroundColor = "green";
					}
				}
			}else{
				//falsche Antwort --> entsprechender Button rot
				switch (num){
					case 0:{
						document.getElementById("A").style.backgroundColor = "red";
						break;
					}
					case 1:{
						document.getElementById("B").style.backgroundColor = "red";
						break;
					}
					case 2:{
						document.getElementById("C").style.backgroundColor = "red";
						break;
					}
					default:{
						document.getElementById("D").style.backgroundColor = "red";
					}
				}
			}
		}
	}

	startQuiz(){
		this.n = 0;
		this.p.init();
		document.getElementById("start").style.visibility  = "hidden";
		document.getElementById("next").style.visibility	= "visible";
		this.nextQuestion();
		document.getElementById("pbar").value = 0;
	}

	nextQuestion(){
		console.log("VIEW: nächste Frage gewählt");
		for(let i=0; i<4; i++) {
			document.querySelectorAll("#answer > *")[i].style.backgroundColor = "";
		}
		let question = this.p.getQuestion();
		if(question == 0){
			document.getElementById("task").innerHTML = "Das Quiz ist beendet. Sie haben " + this.n + " von 10 Fragen richtig beantwortet.";
			for(let i=0; i<4; i++){
				document.querySelectorAll("#answer > *")[i].innerHTML = "";
			}
			document.getElementById("next").style.visibility  = "hidden";
			document.getElementById("start").style.visibility = "visible";
		}else {
			if(a == "m") {
				katex.render(question, task, {
					throwOnError: false
				});
				for (let i=0; i<4; i++) {
					katex.render(this.p.getAnswer(i), document.querySelectorAll("#answer > *")[i], {
						throwOnError: false
					});
				}
			}else if(a == "a"){
				document.getElementById("task").innerHTML = question;
				for(let i=0; i<4; i++){
					document.querySelectorAll("#answer > *")[i].innerHTML = this.p.getAnswer(i);
				}
			}
		}
	}
}


//Presenter
class Presenter{

	//Array verbleibender Fragen
	restQuestions;

	//aktuelle gezogene Frage
	index;

	//Antworten auf aktuelle Frage
	answers;

	//Array verbleibender Antworten
	restAnswers;

	//aktuell richtige Antwort
	rightAnswer;


	constructor(){
		this.init();
		console.log("PRESENTER: initialisiert");
	}

	init(){
		//Array verbleibender Fragen 0-9 initialisieren
		this.restQuestions  = Array.from(Array(10).keys());
		this.restAnswers	= Array.from(Array(4).keys());
	}

	setModelAndView(m, v){
		this.m = m;
		this.v = v;
	}

	evaluate(chosen){
		console.log("PRESENTER: ausgewählte Antwort " + chosen);
		if(chosen == this.rightAnswer){
			console.log("YEAH");
			return true;
		}else{
			return false;
		}
	}

	getQuestion(){
		if(this.restQuestions.length == 0){
			console.log(this.restQuestions);
			return 0;
		}else{
			let rand;
			rand = Math.floor(Math.random() * this.restQuestions.length);
			this.index = this.restQuestions.splice(rand, 1);
			console.log("PRESENTER: nächste zufällige Frage");
			this.answers = this.m.getAnswers(this.index);
			this.restAnswers = Array.from(Array(4).keys());
			return this.m.getTasks(this.index);
		}
	}

	getAnswer(i){
		let rand;
		let aNum;
		rand = Math.floor(Math.random() * this.restAnswers.length);
		aNum = this.restAnswers.splice(rand, 1);
		if (aNum == 0) {
			this.rightAnswer = i;
		}
		console.log("PRESENTER: nächste zufällige Antwort " + this.answers[aNum]);
		return this.answers[aNum];
	}
}
