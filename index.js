(function() {
	var binding = require('./build/Release/binding.node');

	module.exports = {
		parse: function(str) {
			str = binding.toJSON(str);

			if(!str) {
				throw new Error('Unable to convert given string to PyDict.');
			}

			return JSON.parse(str);
		},
		toJSON: binding.toJSON
	};
})();

