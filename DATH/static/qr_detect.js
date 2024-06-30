$(document).ready(function(){
function sendRequest(){
    $.ajax({
        url: '',
        type: "POST",
        dataType: 'json',
        contentType: 'application/json',
        success:
        function(result){
        //const url = getCurrentURL();
        //const new_url =url.concat("display_data");
        if(result.status == 'piled' ){
        location.replace(("/customer_piled"));}
        },
        complete:function(result){
                  setTimeout(sendRequest,3000);}
});
}
 $(document).ready(function()
  {
     setTimeout(sendRequest,3000);
    });
});