import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:flutter/material.dart';
import 'package:google_sign_in/google_sign_in.dart';

class DataBaseService {
 final FirebaseFirestore firebaseFirestore = FirebaseFirestore.instance;
  Future<String> getName() async {
   String strName = "";
   QuerySnapshot result = await firebaseFirestore
   .collection('Users')
   .get();

   DocumentSnapshot document = result.docs[0];

  if (document.get('name') != "") {
    strName = document.get('name');
  }

  return strName;
 }
}