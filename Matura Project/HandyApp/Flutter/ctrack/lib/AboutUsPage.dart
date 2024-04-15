// ignore: file_names
import 'package:flutter/material.dart';
import 'package:flutter/services.dart' show rootBundle;


//=================================About Us Page==============================================

class AboutUsPage extends StatefulWidget{
  const AboutUsPage({super.key});

  @override
  // ignore: library_private_types_in_public_api
  _AboutUsPage createState() => _AboutUsPage();
}


class _AboutUsPage extends State<AboutUsPage>{

String data = '';

  fetchFileData() async{
    String responseText;
    responseText = await rootBundle.loadString('assets/AboutUs.txt'); 

    setState((){
      data = responseText;
    });
  }

  @override
  void initState() {
    fetchFileData();
    super.initState();
  }

  @override 
  Widget build (BuildContext context){
    return Scaffold(

      appBar: AppBar(           
          title: const Text(
            'About Us',  //Text der in Leiste steht
            textDirection: TextDirection.ltr, //Wichtig: sonst "No Directional Widget found"

            style: TextStyle( //Anpassungen für Text
              fontSize: 25,                 
            ),
          ),

          shape: const RoundedRectangleBorder(    //Leiste abrunden
          borderRadius: BorderRadius.only(
              bottomRight: Radius.circular(25),
              bottomLeft: Radius.circular(25)
              ),
          ),

          toolbarHeight: 40,    //Höhe verschieben
          backgroundColor: const Color.fromARGB(255, 89, 225, 59),  //Farbe ändern
        ),

        body: Center(

          child: Padding(
            padding: const EdgeInsets.all(8.0),
            child: Text(data,style: const TextStyle(fontSize: 13.5),),

          ),
      ),
    );
  }
}

//============================================================================================