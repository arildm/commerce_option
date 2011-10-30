Commerce Product Options
--------------------------------------

This module adds customizable products option to the "add to cart" form. The approach is generic, 
so all field types / widget can be used. This module is works only for drupal commerce. 

The module provides a field called option set. You can assign to your commerce product. 
Each option set consists of a list of fields. If a product is connected to a option set the add to cart form 
will be extended by the fields of the option sets.

Sponsored by www.customweb.ch

We use the Commerce Product Attributes module to show the selected options in the shopping cart. 
If the customer clicks in the shopping cart on the product link 
(if the option is activated in the Commerce Product Attributes module), then she / he can edit the 
previous selected or entered option set fields.

Setup:
---------------
1) Setup an option set (Store > Product > Product Set)
2) Setup a product type with a product set reference (separate module "Option Set Reference" 
    is needed, it is included in this package).
3) Add a new product of the above mentioned product type.
4) Select the created product set in the product and save the product.

When and when not to use
-------------------------------------

Please read this carefully. You should use this module without understanding the consequences.

With plain Commerce, you'd add fields on product types for variations such as size and colour, 
and then create one product for each possible combination. Your product node then needs to 
reference all the products you created that fit together to provide a single 'product' as perceived 
by the customer.

B. with commerce option, you create option sets, which then can have multiple fields. 
You then add an option set reference field to your product. This then means that when you see the 
product 'add to cart' form on the product display node, you get the fields from the option set(s) applied 
to the product displayed.

Example:

- make an option set 'size'
- add a field on it 'size' (the names get repetitive...)
- add select options S, M, X, XL etc
- add an option set ref field to your product type(s)
- the product display node now shows the 'size' field.

It is important to understand that this approach means, you do not have SKU's / Products for each of the
possible options. Therefore you cannot use commerce functionality that is based on this distinction. The
most prominent one you can think of, is stock control. But others may be affected. So use this module
only, if you do not care about these functions.