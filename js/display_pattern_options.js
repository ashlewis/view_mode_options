(function($) {

    Drupal.behaviors.displayPatternOptions = {
        attach: function (context, settings) {

            var

                init = function(){
                    $('#edit-row-options-view-mode, #edit-build-mode').change(function(){

                        var view_mode = $(this).val();

                        $.get(
                            '/display_pattern_options/options/'+ view_mode,

                            function(data){

                                if (data) {

                                    $.each(data, function(label, variations) {

                                        var options = '<option value="default">- Default -</option>';

                                        $.each(variations, function(key, value) {
                                            options += '<option value="'+ key +'">'+ value +'</option>';
                                        });

                                        $('#edit-row-options-'+ label +', #edit-display-pattern-options').html(options);
                                    });
                                }
                            },

                            'json'
                        );

                    });

                };

            init();
        }
    };

})(jQuery);