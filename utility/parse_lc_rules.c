/*================================================================
*   Copyright (C) 2014 All rights reserved.
*   
*   FileName：parse_lc_rules.c
*   Author  ：Carl
*   Creat Date：2014.03.13
*   Description：
*
================================================================*/

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

typedef enum tagLC_RULES
{
    AVAILABLE,
    UNAVAILABLE
} LC_RULE;

typedef enum tagLC_PROTOCOL
{
    UNKOWN,
    CMD1,
    CMD2,
    CMD3,
    MAX_PROTOCOL
} LC_PROTOCOL;

static g_lc_rules[MAX_PROTOCOL] = {};

char *rules_example = "getsysinfo=no\r\ninner_protocol=yes\r\n";

void lc_set_cmd_rules(char *rule_buf);
LC_RULE lc_check_rules_type(char *rule);
LC_PROTOCOL lc_check_protocol_type(char **protocol);
void lc_set_cmd_rule(LC_PROTOCOL protocol, LC_RULE rule);
LC_RULE lc_get_cmd_rule(LC_PROTOCOL protocol);
void lc_set_global_ctrl(char *rule);
static int g_lc_net_filter_method = 0;


int main(int argc ,char **argv)
{

    lc_set_cmd_rules(rules_example);
    printf("\n");
    int i = 0;
    for (i=0; i<MAX_PROTOCOL; i++)
    {
        printf("g_lc_rules[%d]:%d\n", i, g_lc_rules[i]);
    }
    printf("g_global_ctrl: %d\n", g_lc_net_filter_method);

    return 0;
}

void lc_set_cmd_rules(char *rule_buf)
{
    char rule[100] = {}; //to store one rule ,eg cmd1=yes
    int32_t rule_len = 0;
    char *pstart = NULL;
    char *pend = NULL;
    char rule_key_array[20] = {};
    char *rule_key = rule_key_array;
    char rule_value[20] = {};
    char *pch = NULL;
    LC_PROTOCOL lc_pro;
    LC_RULE lc_rule;
    const char *sep1="\r\n";
    const char *sep2="=";

    pstart=rule_buf;
    do 
    {
        memset(rule, 0, sizeof(rule));
        memset(rule_key_array, 0, sizeof(rule_key_array));
        memset(rule_value, 0, sizeof(rule_value));
        rule_key = rule_key_array;
        pend = strstr(pstart, sep1);
        if (NULL == pend)
            break;
        rule_len = pend - pstart;
        memcpy(rule, pstart, rule_len);
        printf("rule:%s\n", rule);

        //here to parse further frome 'cmd=yes' to 'cmd' 'yes'
        pch = strtok(rule, sep2);
        if (NULL != pch)
        {
            strncpy(rule_key_array, pch, sizeof(rule_key_array));
            printf("rule_key:%s\n", rule_key);
        }
        lc_pro = lc_check_protocol_type((char **)&rule_key);
        pch = strtok(NULL, sep2);
        if (NULL != pch)
        {
            strncpy(rule_value, pch, sizeof(rule_value));
            printf("rule_value:%s\n", rule_value);
        }
        lc_rule = lc_check_rules_type(rule_value);
        if (UNKOWN != lc_pro)
        {
            lc_set_cmd_rule(lc_pro, lc_rule);
        }
        else if (0 == strcmp(rule_key, "lc_global_ctrl"))
        {
            lc_set_global_ctrl(rule_value);
        }
        
    } while (strlen(pend) > strlen(sep1) && (pstart = pend + strlen(sep1)));
}

void lc_set_cmd_rule(LC_PROTOCOL protocol, LC_RULE rule)
{
    g_lc_rules[protocol] = rule;
}

LC_RULE lc_get_cmd_rule(LC_PROTOCOL protocol)
{
    return g_lc_rules[protocol];
}

LC_RULE lc_check_rules_type(char *rule)
{
    if (strcasecmp(rule, "yes") == 0)
        return AVAILABLE;
    else if (strcasecmp(rule, "no") == 0)
        return UNAVAILABLE;
}

LC_PROTOCOL lc_check_protocol_type(char **protocol)
{
    if (strcasecmp(*protocol, "getsysinfo") == 0)
        return CMD1;
    else if (strcasecmp(*protocol, "inner_protocol") == 0)
        return CMD2;
    else
        return UNKOWN;
}


void lc_set_global_ctrl(char *rule)
{
    if (0 == strcmp(rule, "all"))
    {
        g_lc_net_filter_method = 1;
    }
    else if (0 == strcmp(rule, "lan"))
    {
        g_lc_net_filter_method = 2;
    }
    else if (0 == strcmp(rule, "localhost"))
    {
        g_lc_net_filter_method = 3;
    }
    else
    {
        g_lc_net_filter_method = 4;
    }
}
