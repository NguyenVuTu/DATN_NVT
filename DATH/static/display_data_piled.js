
$(document).ready(function(){
function sendRequest(){
    $.ajax({
        url: '',
        type: "POST",
        dataType: 'json',
        contentType: 'application/json',
        success:
        function(result){
        $('#cccd').text(result.cccd);$('#name').text(result.name);$('#born').text(result.born);
        $('#gender').text(result.gender);
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
