#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static ngx_int_t ngx_http_cache_key_filter_init(ngx_conf_t *cf);


// Dummy function
static char *
ngx_http_cache_key_filter_set(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
  return NGX_CONF_OK;
}

static ngx_command_t  ngx_http_cache_key_filter_commands[] = {

  { ngx_string("cache_key_filter"),
  NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,
  ngx_http_cache_key_filter_set,
  0,
  0,
  NULL },

  ngx_null_command
};


static ngx_http_module_t  ngx_http_cache_key_filter_module_ctx = {
  NULL,                                         /* proconfiguration */
  ngx_http_cache_key_filter_init,               /* postconfiguration */

  NULL,                                         /* create main configuration */
  NULL,                                         /* init main configuration */

  NULL,                                         /* create server configuration */
  NULL,                                         /* merge server configuration */

  NULL,                         /* create location configuration */
  NULL                          /* merge location configuration */
};

ngx_module_t  ngx_http_cache_key_filter_module = {
  NGX_MODULE_V1,
  &ngx_http_cache_key_filter_module_ctx, /* module context */
  ngx_http_cache_key_filter_commands,    /* module directives */
  NGX_HTTP_MODULE,                       /* module type */
  NULL,                                  /* init master */
  NULL,                                  /* init module */
  NULL,                                  /* init process */
  NULL,                                  /* init thread */
  NULL,                                  /* exit thread */
  NULL,                                  /* exit process */
  NULL,                                  /* exit master */
  NGX_MODULE_V1_PADDING
};


static ngx_http_output_header_filter_pt ngx_http_next_header_filter;


static ngx_int_t
ngx_http_cache_key_filter(ngx_http_request_t *r)
{
    if(r->cache == NULL){
        return ngx_http_next_header_filter(r);
    }

    u_char *hex = ngx_pnalloc(r->pool, 32);
    if (hex == NULL) return NGX_ERROR;

    ngx_hex_dump(hex, r->cache->key, NGX_HTTP_CACHE_KEY_LEN);

    ngx_table_elt_t *h = ngx_list_push(&r->headers_out.headers);
    if (h == NULL) return NGX_ERROR;

    h->hash = 1;
    ngx_str_set(&h->key, "X-Cache-Key");
    h->value.data = hex;
    h->value.len = 32;

    return ngx_http_next_header_filter(r);
}


static ngx_int_t
ngx_http_cache_key_filter_init(ngx_conf_t *cf)
{

  ngx_http_next_header_filter = ngx_http_top_header_filter;
  ngx_http_top_header_filter = ngx_http_cache_key_filter;

  return NGX_OK;
}
