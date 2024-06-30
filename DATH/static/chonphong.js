function room1_3h(){
    location.href = '/chonphong/room1_3h';
}
function room1_12h(){
    location.href = '/chonphong/room1_12h';
}
function room1_overnight(){
    location.href = '/chonphong/room1_overnight';
}
function room2_3h(){
    location.href = '/chonphong/room2_3h';
}
function room2_12h(){
    location.href = '/chonphong/room2_12h';
}
function room2_overnight(){
    location.href = '/chonphong/room2_overnight';
}

$(document).ready(function(){
function sendRequest(){
    $.ajax({
        url: '',
        type: "POST",
        dataType: 'json',
        contentType: 'application/json',
        success:
        function(result){
        if (result.status1 == 'exist'){
            $('#button1').css("background-color","red");
        }else{
            $('#button1').css("background-color","green");
        }
        if (result.status2 == 'exist'){
            $('#button2').css("background-color","red");
        }else{
            $('#button2').css("background-color","green");
        }
        },
        complete:function(result){
                  setTimeout(sendRequest,1000);}
});
}
 $(document).ready(function()
  {
     setTimeout(sendRequest,1000);
    });
});