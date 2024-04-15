import 'package:flutter/material.dart';
import 'package:flutter_map/flutter_map.dart';
import 'package:latlong2/latlong.dart';


void main() {
  runApp( MyHomePage());
}

// ignore: use_key_in_widget_constructors
class MyHomePage extends StatefulWidget {
  @override
  // ignore: library_private_types_in_public_api
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      home: Scaffold(
        appBar: AppBar(
          title: const Text(
            'C-Track 2.0',
            textDirection: TextDirection.ltr,
            style: TextStyle(fontSize: 30), 
          ),

          centerTitle: true,    //Titel in Mitte verschieben
          toolbarHeight: 30,    //Höhe Verschieben
          backgroundColor: const Color.fromARGB(255, 149, 244, 128),  //Farbe ändern

        ),

        body: content(),

      ),

    ); 
    
  }

  Widget content() {
    return FlutterMap(
      options: const MapOptions(
        initialCenter: LatLng(47.820317, 13.043026),
        initialZoom: 11,
        interactionOptions: 
          	InteractionOptions(flags: ~InteractiveFlag.doubleTapZoom),
      ),
      children: [
            TileLayer(
            urlTemplate: 'https://tile.openstreetmap.org/{z}/{x}/{y}.png',
            userAgentPackageName: 'dev.fleaflet.flutter_map.example',
            ),
      ],
    );
  }
}
