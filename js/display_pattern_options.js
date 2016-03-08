(function($) {

    Drupal.behaviors.displayPatternOptions = {
        attach: function (context, settings) {

            var

                init = function(){
                    $('#edit-row-options-view-mode').change(function(){

                        var view_mode = $('#edit-row-options-view-mode').val();

                        $.get(
                            '/display_pattern_options/options/'+ view_mode,

                            function(data){
                                var options = '';

                                $.each(data, function( key, value ) {
                                    options += '<option value="'+ key +'">'+ value +'</option>';
                                });

                                $('#edit-row-options-display-pattern-options').html(options);
                            },

                            'json'
                        );

                    });
                };

            init();
        }
    };

})(jQuery);