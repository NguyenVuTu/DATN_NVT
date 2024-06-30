$(document).ready(function(){
function sendRequest(){
    $.ajax({
        url: '',
        type: "POST",
        dataType: 'json',
        contentType: 'application/json',
        data: JSON.stringify({room: $('#room').val(), time: $('#time').val(), verify: $('#verify').val(),  final_verify: $('#final_verify').val()}),
        success:
        function(result){
        var notice = "Đã có khách hàng có số CCCD là: " + result.pile_cccd + ", số điện thoại: " +result.pile_phone_number + " thanh toán tiền cọc cho: " + result.pile_room + ", ngày lưu trú: " + result.pile_start_day +
        ", số ngày lưu trú: " + result.pile_leng_stay + ", ngày trả phòng: " + result.pile_end_day
        + ", số tiền: " + result.pile_total_money;
        var notice2 = "Khách hàng ở :" + result.final_room_id + " check-in ngày: " + result.final_check_in + " đã thanh toán số tiền còn lại là: " +  result.remain_payment + " VND";
        $('#room1_cccd').text(result.room1_cccd);$('#room1_name').text(result.room1_name);$('#room1_id').text(result.room1_id);
        $('#room1_check_in').text(result.room1_check_in);$('#room1_check_out').text(result.room1_check_out);
        $('#room2_cccd').text(result.room2_cccd);$('#room2_name').text(result.room2_name);$('#room2_id').text(result.room2_id);
        $('#room2_check_in').text(result.room2_check_in);$('#room2_check_out').text(result.room2_check_out);
//        if(result.room == 'room1' || result.room == 'room2'){
//            if  (confirm("Đã có khách hàng chọn phòng") == true){
//            $('#room').val(result.room);
//            $('#time').val(result.time);
//                }
//                }else{
//                $('#room').val('refuse');
//                $('#time').val('refuse');}
        if(result.piled_request == "YES")
        {
            if  (confirm(notice2) == true){
                $('#final_verify').val('COMPLETE');
            }
        }
        else{
                $('#final_verify').val('refuse');
            }

        //-----------------------------------
        if(result.custom_request == "YES")
        {
            if  (confirm(notice) == true){
                $('#verify').val('COMPLETE');
            }
        }
        else{
                $('#verify').val('refuse');
            }

        }
        ,
        complete:function(result){
                  setTimeout(sendRequest,1000);}
});
}

 $(document).ready(function()
  {
     setTimeout(sendRequest,1000);
    });
});
