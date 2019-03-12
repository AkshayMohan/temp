<?php

require_once "config.php";

$connHandle = mysqli_connect($DB_HOST, $DB_USER, $DB_PASS, $DB_NAME);
if(mysqli_connect_errno()) {

    die('Failed connecting to database! ERROR : ' . mysqli_connect_errno());
} //Stupid vscode auto-intends if I don't use this fucking bracket.

//If the form is not submitted properly.
if(!isset($_POST['username'], $_POST['password'])) {

    die('ERROR : You must fill in both username and password fields.');
}

//Using prepared statements to get rid of SQL injection.
$stmt = $connHandle->prepare(
    'SELECT `userid`, `userpass` FROM `user` WHERE `username`=?');
if($stmt) {

    $stmt->bind_param('s', $_POST['username']); //Bind the username string to query.
    $stmt->execute(); //Execute the query.


    $stmt->store_result(); //Store the returned result.

    //Check if account exists.
    if($stmt->num_rows > 0) {

        $stmt->bind_result($user_id, $user_pass);
        $stmt->fetch();

        //Not using hashing here to make bruteforce demo easier.

        if($user_pass === $_POST['password']) {

            //Password matches, so it's a sucessful login!
            session_regenerate_id();
            $_SESSION['loggedin'] = TRUE;
            $_SESSION['user_name'] = $_POST['username'];
            $_SESSION['user_id'] = $user_id;

            echo 'Authorized access, welcome ' . $_SESSION['user_name'] . '<br>';
        } else {

            echo 'Incorrect password!<br>';
        }
    } else {

        echo 'ERROR : Username does not exist! Why not register one <a href="register.html"> here</a>?';
    }
    $stmt->close();
}
?>