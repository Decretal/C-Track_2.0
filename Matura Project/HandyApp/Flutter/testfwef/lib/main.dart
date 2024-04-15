import 'package:flutter/material.dart';

void main() {
  runApp(MainApp());
}

class MainApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    home: MyHomePage(),
    
  }
}

class MyHomePage extends StatefulWidget{
  @override
  _MyHomePageState createState() => _MyHomePageState();

}

class _MyHomePageState extends State<MyHomePage> {

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar : AppBar(
        title: Text(
          'Open Street Map in Flutter',
          style: TextStyle(fontSize: 22),
        ),
      ),
      body: content()
    );
  }

  Widget content(){
  return Container();
}
}

