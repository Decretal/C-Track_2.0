// ignore_for_file: unnecessary_new, prefer_const_constructors

import 'package:ctrack/AboutUsPage.dart';
import 'package:ctrack/osm_license.dart';
import 'package:flutter/material.dart';

class NavBar extends StatelessWidget{
  const NavBar ({super.key});

  @override 
  Widget build(BuildContext context) {
    return Drawer(        //Liste erstellen
      child: ListView(
        padding: EdgeInsets.zero,
        children: [

          const DrawerHeader(  //Listen Header
            decoration: BoxDecoration(    //Decoration des Listen Headers
                image: DecorationImage(    //Ctrack Bild einfügen in Header
                  image: AssetImage("assets/ctrack.png"),
                  fit: BoxFit.fill,
                ),  
            ), child: null,  //Listen Header
          ),

          ListTile(                     
            leading: const Icon(Icons.account_circle_rounded),
            title: const Text('About Us'),  
            onTap: () {
              Navigator.push(context, new MaterialPageRoute(builder: (context) => new AboutUsPage()));  //Wenn Tap dann wechselt Seite
            },
          ),

          ListTile(                       //zweiter Button in Liste
            leading: const Icon(Icons.info_outline),
            title: const Text('OSM License'), //Name in Liste
            onTap: () {
              Navigator.push(context, new MaterialPageRoute(builder: (context) => new OSMLicensePage()));  //Wenn Tap dann wechselt Seite
            },
          ),
        ],
      ),
    );
  }
}