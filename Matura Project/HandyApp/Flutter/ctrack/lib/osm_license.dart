// ignore_for_file: unnecessary_new, use_key_in_widget_constructors, library_private_types_in_public_api

import 'package:flutter/material.dart';
import 'package:flutter/services.dart' show rootBundle;


//=============================== OSM License Paige =========================================


class OSMLicensePage extends StatefulWidget{
  @override
  _OSMLicensePage createState() => _OSMLicensePage();
}

class _OSMLicensePage extends State<OSMLicensePage>{

  String data = '';

  fetchFileData() async{
    String responseText;
    responseText = await rootBundle.loadString('assets/License.txt');

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
    return new Scaffold(

      appBar: AppBar(

          title: const Text(
            'OSM License',  //Text der in Leiste steht

            textDirection: TextDirection.ltr, //Wichtig: sonst "No Directional Widget found"

            style: TextStyle( //Anpassungen für Text
              fontSize: 25,                 //Größe: 30            
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
          child: Text(data,style: const TextStyle(fontSize: 12.5),),

        ),
      ),
    );
  }
}

//===========================================================================================