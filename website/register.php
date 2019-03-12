<?php

require_once "config.php";

$connHandle = mysqli_connect($DB_HOST, $DB_USER, $DB_PASS, $DB_NAME);
if(mysqli_connect_errno()) {

    die('Failed connecting to database! ERROR : ' . mysqli_connect_errno());
} //Stupid vscode auto-intends if I don't use this fucking bracket.

//Check if form is filled.
//<input type="password" placeholder="Confirm Password" id="rcpassword" required><br><br>
//if(!isset($_POST['username'], $_POST['password'], $_POST['cpassword'])) {
if(!isset($_POST['username'], $_POST['password'], $_POST['cpassword'])) {

    die('ERROR : Please fill out the forms properly to register an account.');
}
if(!($_POST['password'] === $_POST['cpassword'])) {

    die('ERROR : Passwords do not match!');
}
//Check if an account exists!
$stmt = $connHandle->prepare(
    'SELECT `userid` FROM `user` WHERE `username`=?');
if($stmt) {

    $stmt->bind_param('s', $_POST['username']);
    $stmt->execute();

    $stmt->store_result();
    //if there are any rows, it means an account exists!

    if($stmt->num_rows() != 0) {

        echo 'ERROR : That username is not available!<br>';
    } else {
        
        $regstmt = $connHandle->prepare(
         'INSERT INTO `user`(`username`, `userpass`) VALUES(?,?)');
         
        $regstmt->bind_param('ss', $_POST['username'], $_POST['password']);
        $regstmt->execute();

        echo 'Account created successfully!<br>';

        $regstmt->close();
    }
    $stmt->close();
}
?>